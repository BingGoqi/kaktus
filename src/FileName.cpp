#include <vsg/all.h>

#include <iostream>
#include <chrono>

std::string vertexShaderSource{ R"(
#version 450
layout(location = 0) in vec3 vsg_Vertex;

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelView;
} pc;

void main()
{
    gl_Position = vec4(vsg_Vertex, 1.0f);
})"
};
std::string fragmentShaderSource{ R"(
#version 450
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushColor {
    layout(offset = 128)vec4 color;
} uColor;

void main()
{
    outColor = uColor.color;
})"
};

int main(int argc, char** argv)
{
    // set up defaults and read command line arguments to override them
    vsg::CommandLine arguments(&argc, argv);

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "vsgTriangle";
    windowTraits->debugLayer = arguments.read({ "--debug", "-d" });

    if (arguments.read({ "--window", "-w" }, windowTraits->width, windowTraits->height)) { windowTraits->fullscreen = false; }

    vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::create(VK_SHADER_STAGE_VERTEX_BIT, "main", vertexShaderSource);
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::create(VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragmentShaderSource);
    if (!vertexShader || !fragmentShader)
    {
        std::cout << "Could not create shaders." << std::endl;
        return 1;
    }

    vsg::VertexInputState::Bindings vertexBindingsDescriptions{
        VkVertexInputBindingDescription{0, sizeof(vsg::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // vertex data
    };

    vsg::VertexInputState::Attributes vertexAttributeDescriptions{
        VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}, // vertex data
    };

    vsg::GraphicsPipelineStates pipelineStates{
        vsg::VertexInputState::create(vertexBindingsDescriptions, vertexAttributeDescriptions),
        vsg::InputAssemblyState::create(),
        vsg::RasterizationState::create(),
        vsg::MultisampleState::create(),
        vsg::ColorBlendState::create(),
        vsg::DepthStencilState::create()
    };

    vsg::PushConstantRanges pushConstantRanges{
        {VK_SHADER_STAGE_VERTEX_BIT, 0, 128}, // projection, view, and model matrices, actual push constant calls automatically provided by the VSG's RecordTraversal
        {VK_SHADER_STAGE_FRAGMENT_BIT, 128, sizeof(vsg::vec4)}
    };

    auto pipelineLayout = vsg::PipelineLayout::create(vsg::DescriptorSetLayouts{}, pushConstantRanges);
    auto graphicsPipeline = vsg::GraphicsPipeline::create(pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates);
    auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create(graphicsPipeline);

    // create StateGroup as the root of the scene/command graph to hold the GraphicsPipeline
    auto scenegraph = vsg::StateGroup::create();
    scenegraph->add(bindGraphicsPipeline);

    // set up vertex arrays
    auto vertices = vsg::vec3Array::create(
        { {-0.5f, -0.5f, 0.5f},
         {0.5f, -0.5f, 0.5f},
         {0.5f, 0.5f, 0.5f} });

    // in vulkan NDC, x axis point to right, y axis point to bottom, z point in the screen, it is right handed system
    // in this section, use indices to let the traingle point out. 
    auto indices = vsg::ushortArray::create(
        { 0, 2, 1 });

    auto colorData = vsg::vec4Array::create({ {1.0, 0.0, 0.0, 1.0} });
    auto pushConstant = vsg::PushConstants::create(VK_SHADER_STAGE_FRAGMENT_BIT, 128, colorData);

    // setup geometry
    auto drawCommands = vsg::Commands::create();
    drawCommands->addChild(pushConstant);
    drawCommands->addChild(vsg::BindVertexBuffers::create(0, vsg::DataList{ vertices }));
    drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
    drawCommands->addChild(vsg::DrawIndexed::create(3, 1, 0, 0, 0));

    scenegraph->addChild(drawCommands);

    // create the viewer and assign window(s) to it
    auto viewer = vsg::Viewer::create();

    auto window = vsg::Window::create(windowTraits);
    if (!window)
    {
        std::cout << "Could not create window." << std::endl;
        return 1;
    }

    viewer->addWindow(window);

    auto camera = vsg::Camera::create();

    //viewer->addEventHandler(vsg::TrackingViewMatrix::create(camera));

    auto commandGraph = vsg::createCommandGraphForView(window, camera, scenegraph);
    viewer->assignRecordAndSubmitTaskAndPresentation({ commandGraph });

    // compile the Vulkan objects
    viewer->compile();

    // assign a CloseHandler to the Viewer to respond to pressing Escape or the window close button
    viewer->addEventHandlers({ vsg::CloseHandler::create(viewer) });

    viewer->compile();

    while (viewer->advanceToNextFrame())
    {
        viewer->handleEvents();

        float time = std::chrono::duration<float, std::chrono::seconds::period>(viewer->getFrameStamp()->time - viewer->start_point()).count();
        colorData->at(0).r = 0.5f * sin(time) + 0.5f;

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }

    // clean up done automatically thanks to ref_ptr<>
    return 0;
}