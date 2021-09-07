#include "RenderPipeline.h"
#include "Entity/Scene.h"
#include "Entity/Entity.h"
#include "Entity/Components/CCamera.h"
#include "Entity/Components/CLight.h"
#include "Entity/Components/CRenderable.h"
#include "Entity/Components/CTransform.h"
#include "Resource/GpuBuffer.h"
#include "Resource/Texture.h"
#include "Resource/Material.h"
#include "Resource/RenderTarget.h"
#include "GearEngine.h"
#include "Renderer/Renderer.h"

#define MAX_NUM_VIEWS 2

namespace gear {
    RenderPipeline::RenderPipeline() {
        _view_ub = new UniformBuffer(sizeof(ViewUniforms));
        _renderable_ub = new UniformBuffer(sizeof(RenderableUniforms) * 1000);

        for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; ++i) {
            // 阴影贴图的默认分辨率为1024
            gear::Texture::Builder tex_builder;
            tex_builder.SetWidth(1024);
            tex_builder.SetHeight(1024);
            tex_builder.SetFormat(blast::FORMAT_D24_UNORM_S8_UINT);
            _shadow_maps[i] = tex_builder.Build();
        }

        _debug_lines.resize(MAX_DEBUG_LINES * 14);

        glm::mat4 identity_matrix = glm::mat4(1.0f);
        _debug_ub = new UniformBuffer(sizeof(RenderableUniforms));
        _debug_ub->Update(&identity_matrix, offsetof(RenderableUniforms, model_matrix), sizeof(glm::mat4));
        _debug_ub->Update(&identity_matrix, offsetof(RenderableUniforms, normal_matrix), sizeof(glm::mat4));

        gear::VertexBuffer::Builder builder;
        builder.SetVertexCount(MAX_DEBUG_POINTS);
        builder.SetAttribute(blast::SEMANTIC_POSITION, blast::FORMAT_R32G32B32_FLOAT);
        builder.SetAttribute(blast::SEMANTIC_COLOR, blast::FORMAT_R32G32B32A32_FLOAT);

        builder.SetVertexCount(MAX_DEBUG_LINES);
        _debug_line_vb = builder.Build();
    }

    RenderPipeline::~RenderPipeline() {
        SAFE_DELETE(_view_ub);
        SAFE_DELETE(_renderable_ub);
        SAFE_DELETE(_debug_ub);
        SAFE_DELETE(_debug_line_vb);

        for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; ++i) {
            SAFE_DELETE(_shadow_maps[i]);
        }
    }

    void RenderPipeline::SetScene(Scene* scene) {
        _scene = scene;
    }

    void RenderPipeline::Exec() {
        Renderer* renderer = gEngine.GetRenderer();

        if (!_scene) {
            return;
        }

        _num_debug_lines = 0;
        _num_views = 0;
        _num_lights = 0;
        _num_renderables = 0;
        _num_ui_renderables = 0;
        _num_common_renderables = 0;

        if (_renderables.size() < _scene->_entities.size()) {
            _renderables.resize(_scene->_entities.size());
            _ui_renderables.resize(_scene->_entities.size());
            _common_renderables.resize(_scene->_entities.size());
        }

        if (_renderable_ub->GetSize() < sizeof(RenderableUniforms) * _scene->_entities.size()) {
            UniformBuffer* old_renderable_ub = _renderable_ub;
            SAFE_DELETE(old_renderable_ub);
            _renderable_ub = new UniformBuffer(sizeof(RenderableUniforms) * _scene->_entities.size());
        }

        if (_lights.size() < _scene->_entities.size()) {
            _lights.resize(_scene->_entities.size());
        }

        if (_views.size() < MAX_NUM_VIEWS) {
            _views.resize(MAX_NUM_VIEWS);
        }

        for (auto entity : _scene->_entities) {
            if (entity->HasComponent<CCamera>()) {
                if (_num_views < MAX_NUM_VIEWS) {
                    CCamera* ccamera = entity->GetComponent<CCamera>();

                    if (ccamera->GetDisplay()) {
                        // 设置display_fb
                        if (ccamera->GetRenderTarget() == nullptr) {
                            _display_fb.is_screen_fb = true;
                            _display_fb.clear_value.flags = blast::CLEAR_NONE;
                            _display_fb.width = renderer->GetWidth();
                            _display_fb.height = renderer->GetHeight();
                            std::get<0>(_display_fb.colors[0]) = renderer->GetColor();
                            std::get<1>(_display_fb.colors[0]) = 0;
                            std::get<2>(_display_fb.colors[0]) = 0;
                            std::get<0>(_display_fb.depth_stencil) = renderer->GetDepthStencil();
                            std::get<1>(_display_fb.depth_stencil) = 0;
                            std::get<2>(_display_fb.depth_stencil) = 0;
                        } else {
                            _display_fb.clear_value.flags = blast::CLEAR_NONE;
                            _display_fb.width = ccamera->GetRenderTarget()->GetWidth();
                            _display_fb.height = ccamera->GetRenderTarget()->GetHeight();
                            for (int i = 0; i < TARGET_COUNT; ++i) {
                                auto c = ccamera->GetRenderTarget()->GetColor(i);
                                std::get<0>(_display_fb.colors[i]) = std::get<0>(c)->GetTexture();
                                std::get<1>(_display_fb.colors[i]) = std::get<1>(c);
                                std::get<2>(_display_fb.colors[i]) = std::get<2>(c);
                            }
                            auto d = ccamera->GetRenderTarget()->GetDepthStencil();
                            std::get<0>(_display_fb.depth_stencil) = std::get<0>(d)->GetTexture();
                            std::get<1>(_display_fb.depth_stencil) = std::get<1>(d);
                            std::get<2>(_display_fb.depth_stencil) = std::get<2>(d);
                        }

                        _display_camera_info.zn = ccamera->GetNear();
                        _display_camera_info.zf = ccamera->GetFar();
                        _display_camera_info.model = ccamera->GetModelMatrix();
                        _display_camera_info.view = ccamera->GetViewMatrix();
                        _display_camera_info.projection = ccamera->GetProjMatrix();
                        _display_camera_info.position = GetTranslate(_display_camera_info.model);

                        _view_ub->Update(&_display_camera_info.view, offsetof(ViewUniforms, view_matrix), sizeof(glm::mat4));
                        _view_ub->Update(&_display_camera_info.projection, offsetof(ViewUniforms, proj_matrix), sizeof(glm::mat4));
                    }

                    if (ccamera->GetMain()) {
                        _main_camera_info.zn = ccamera->GetNear();
                        _main_camera_info.zf = ccamera->GetFar();
                        _main_camera_info.model = ccamera->GetModelMatrix();
                        _main_camera_info.view = ccamera->GetViewMatrix();
                        _main_camera_info.projection = ccamera->GetProjMatrix();
                        _main_camera_info.position = GetTranslate(_main_camera_info.model);
                    }

                    _num_views++;
                }
            }

            if (entity->HasComponent<CLight>()) {
                _light_info.has_direction_light = true;
                _light_info.sun_direction = entity->GetComponent<CTransform>()->GetFrontVector();
                glm::normalize(_light_info.sun_direction);
                _num_lights++;
            }

            if (entity->HasComponent<CRenderable>()) {
                CRenderable* crenderable = entity->GetComponent<CRenderable>();
                glm::mat4 model_matrix = entity->GetComponent<CTransform>()->GetWorldTransform();
                glm::mat4 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
                // 设置renderable
                _renderables[_num_renderables].renderable_ub_size = sizeof(RenderableUniforms);
                _renderables[_num_renderables].renderable_ub_offset = _num_renderables * sizeof(RenderableUniforms);
                _renderables[_num_renderables].renderable_ub = _renderable_ub;
                _renderables[_num_renderables].bone_ub = crenderable->_bb;
                _renderables[_num_renderables].primitives.clear();
                for (uint32_t i = 0; i < crenderable->_primitives.size(); ++i) {
                    _renderables[_num_renderables].primitives.push_back(crenderable->_primitives[i]);
                    BBox& bbox = _renderables[_num_renderables].primitives[i].bbox;
                    bbox.bb_min = TransformPoint(bbox.bb_min, model_matrix);
                    bbox.bb_max = TransformPoint(bbox.bb_max, model_matrix);
                }

                // 更新renderable_ub
                _renderable_ub->Update(&model_matrix, _num_renderables * sizeof(RenderableUniforms) + offsetof(RenderableUniforms, model_matrix), sizeof(glm::mat4));
                _renderable_ub->Update(&normal_matrix, _num_renderables * sizeof(RenderableUniforms) + offsetof(RenderableUniforms, normal_matrix), sizeof(glm::mat4));

                // 归类
                RenderableType type = crenderable->GetRenderableType();
                if (type == RENDERABLE_COMMON) {
                    _common_renderables[_num_common_renderables] = _num_renderables;
                    _num_common_renderables++;
                } else if (type == RENDERABLE_UI) {
                    _ui_renderables[_num_ui_renderables] = _num_renderables;
                    _num_ui_renderables++;
                }

                _num_renderables++;
            }
        }

        // 若当前场景不存在相机或没有renderable，则不进行渲染
        if (_num_views == 0 || _num_renderables == 0) {
            return;
        }

        // 管线开始前的初始化
        _dc_head = 0;
        renderer->BindFrameUniformBuffer(_view_ub->GetHandle(), _view_ub->GetSize(), 0);

        // shadow
        ExecShadowStage();

        // render comon renderable
        uint32_t common_dc_head = _dc_head;
        uint32_t num_common_dc = 0;
        for (uint32_t i = 0; i < _num_common_renderables; ++i) {
            Renderable* rb = &_renderables[_common_renderables[i]];
            for (uint32_t j = 0; j < rb->primitives.size(); ++j) {
                RenderPrimitive* rp = &rb->primitives[j];

                MaterialVariant::Key material_variant = 0;
                if (_light_info.has_direction_light) {
                    material_variant |= MaterialVariant::DIRECTIONAL_LIGHTING;
                }

                if (rp->receive_shadow) {
                    material_variant |= MaterialVariant::SHADOW_RECEIVER;
                }

                _dc_list[common_dc_head + num_common_dc] = {};
                _dc_list[common_dc_head + num_common_dc].key = 0;
                _dc_list[common_dc_head + num_common_dc].renderable_ub = rb->renderable_ub->GetHandle();
                _dc_list[common_dc_head + num_common_dc].renderable_ub_size = rb->renderable_ub_size;
                _dc_list[common_dc_head + num_common_dc].renderable_ub_offset = rb->renderable_ub_offset;

                if (rb->bone_ub) {
                    material_variant |= MaterialVariant::SKINNING_OR_MORPHING;
                    _dc_list[common_dc_head + num_common_dc].bone_ub = rb->bone_ub->GetHandle();
                } else {
                    _dc_list[common_dc_head + num_common_dc].bone_ub = nullptr;
                }

                _dc_list[common_dc_head + num_common_dc].vertex_layout = rp->vb->GetVertexLayout();
                _dc_list[common_dc_head + num_common_dc].vb = rp->vb->GetHandle();

                _dc_list[common_dc_head + num_common_dc].ib_count = rp->count;
                _dc_list[common_dc_head + num_common_dc].ib_offset = rp->offset;
                _dc_list[common_dc_head + num_common_dc].ib_type = rp->ib->GetIndexType();
                _dc_list[common_dc_head + num_common_dc].ib = rp->ib->GetHandle();

                _dc_list[common_dc_head + num_common_dc].topo = rp->topo;

                _dc_list[common_dc_head + num_common_dc].render_state = rp->mi->GetMaterial()->GetRenderState();

                _dc_list[common_dc_head + num_common_dc].vs = rp->mi->GetMaterial()->GetVertShader(material_variant);
                _dc_list[common_dc_head + num_common_dc].fs = rp->mi->GetMaterial()->GetFragShader(material_variant);

                if (rp->mi->GetUniformBuffer()) {
                    _dc_list[common_dc_head + num_common_dc].material_ub = rp->mi->GetUniformBuffer()->GetHandle();
                    _dc_list[common_dc_head + num_common_dc].material_ub_size = rp->mi->GetUniformBuffer()->GetSize();
                    _dc_list[common_dc_head + num_common_dc].material_ub_offset = 0;
                } else {
                    _dc_list[common_dc_head + num_common_dc].material_ub = nullptr;
                    _dc_list[common_dc_head + num_common_dc].material_ub_size = 0;
                    _dc_list[common_dc_head + num_common_dc].material_ub_offset = 0;
                }

                for (uint32_t k = 0; k < rp->mi->GetGfxSamplerGroup().size(); ++k) {
                    std::get<0>(_dc_list[common_dc_head + num_common_dc].samplers[4 + k]) = rp->mi->GetGfxSamplerGroup().at(k).first->GetTexture();
                    std::get<1>(_dc_list[common_dc_head + num_common_dc].samplers[4 + k]) = rp->mi->GetGfxSamplerGroup().at(k).second;
                }
                num_common_dc++;
            }
        }
        _dc_head += num_common_dc;

        // 排序
        std::sort(&_dc_list[common_dc_head], &_dc_list[common_dc_head] + num_common_dc);

        renderer->BindFramebuffer(_display_fb);
        for (uint32_t i = common_dc_head; i < common_dc_head + num_common_dc; ++i) {
            renderer->ExecuteDrawCall(_dc_list[i]);
        }
        renderer->UnbindFramebuffer();

        // debug
        ExecDebugStage();

        // ui
        ExecUiStage();
    }
}