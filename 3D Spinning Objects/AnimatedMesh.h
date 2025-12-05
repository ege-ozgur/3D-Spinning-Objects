#pragma once
#include <vector>
#include <string>
#include <d3d12.h>
#include "Core.h"
#include "PSOManager.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Animation.h"
#include "GEMLoader.h"
#include "Vertex.h"
#include "ConstantBuffer.h"
#include "ShaderReflection.h"

class AnimatedMesh
{
public:
    std::vector<Mesh*> meshes;
    Animation animation;

    ShaderManager shaderMgr;
    PSOManager    psoMgr;

    ConstantBuffer vsCB;
    ConstantBufferDescription vsCBDesc;

    int boneCount = 0;

    void load(Core* core, const std::string& filename)
    {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemMeshes;
        GEMLoader::GEMAnimation gemAnim;

        loader.load(filename, gemMeshes, gemAnim);

        for (int i = 0; i < (int)gemMeshes.size(); i++)
        {
            Mesh* mesh = new Mesh();
            std::vector<ANIMATED_VERTEX> vertices;
            vertices.reserve(gemMeshes[i].verticesAnimated.size());

            for (auto& srcV : gemMeshes[i].verticesAnimated)
            {
                ANIMATED_VERTEX v;
                memcpy(&v, &srcV, sizeof(ANIMATED_VERTEX));
                vertices.push_back(v);
            }
            mesh->init(core, vertices, gemMeshes[i].indices);
            meshes.push_back(mesh);
        }

        ID3DBlob* vs = shaderMgr.loadVS("AnimatedVS", "animVertexShader.hlsl");
        ID3DBlob* ps = shaderMgr.loadPS("AnimatedPS", "pixelShader.hlsl");

        psoMgr.createPSO(
            core,
            "AnimatedMeshPSO",
            vs, ps,
            VertexLayoutCache::getAnimatedLayout()
        );

        ConstantBufferLayout layout = ShaderReflection::reflect(vs, "staticMeshBuffer");

        vsCBDesc.name = layout.name;
        vsCBDesc.totalSize = layout.totalSize;

        for (auto& kv : layout.variables)
        {
            ConstantBufferVariable v;
            v.offset = kv.second.offset;
            v.size = kv.second.size;
            vsCBDesc.constantBufferData[kv.first] = v;
        }

        vsCB.init(core, vsCBDesc, 1024);

        memcpy(&animation.skeleton.globalInverse, &gemAnim.globalInverse, sizeof(Matrix));
        boneCount = (int)gemAnim.bones.size();
        animation.skeleton.bones.reserve(boneCount);

        for (auto& b : gemAnim.bones)
        {
            Bone bone;
            bone.name = b.name;
            memcpy(&bone.offset, &b.offset, sizeof(Matrix));
            bone.parentIndex = b.parentIndex;
            animation.skeleton.bones.push_back(bone);
        }

        for (auto& gAnim : gemAnim.animations)
        {
            AnimationSequence seq;
            seq.ticksPerSecond = gAnim.ticksPerSecond;
            for (auto& f : gAnim.frames)
            {
                AnimationFrame frame;
                int count = (int)f.positions.size();
                for (int i = 0; i < count; i++)
                {
                    Vec3 p, s; Quaternion q;
                    memcpy(&p, &f.positions[i], sizeof(Vec3));
                    memcpy(&q, &f.rotations[i], sizeof(Quaternion));
                    memcpy(&s, &f.scales[i], sizeof(Vec3));
                    frame.positions.push_back(p);
                    frame.rotations.push_back(q);
                    frame.scales.push_back(s);
                }
                seq.frames.push_back(frame);
            }
            animation.animations[gAnim.name] = seq;
        }
        OutputDebugStringA("AnimatedMesh::load - OK\n");
    }

    void draw(Core* core,
        AnimationInstance* instance,
        const Matrix& vp,
        const Matrix& w)
    {
        psoMgr.bind(core, "AnimatedMeshPSO");

        vsCB.nextInstance();

        vsCB.update("W", &w, sizeof(Matrix));
        vsCB.update("VP", &vp, sizeof(Matrix));
        vsCB.update("bones", instance->matrices, sizeof(Matrix) * boneCount);

        core->getCommandList()->SetGraphicsRootConstantBufferView(
            0, 
            vsCB.getGPUAddress()
        );

        for (auto m : meshes)
            m->draw(core);
    }
};