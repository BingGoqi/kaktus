#include <vsg/all.h>
#include <vsg/maths/quat.h>
#include <vsgXchange/all.h>

#include <iostream>

int main(int argc, char** argv)
{

    // create the viewer and assign window(s) to it
    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "Text2";
    auto viewer = vsg::Viewer::create();
    auto window = vsg::Window::create(windowTraits);
    if (!window)
    {
        std::cout << "Could not create window." << std::endl;
        return 1;
    }
    //vsg::GeometryInfo geomInfo;
    viewer->addWindow(window);
    auto scene = vsg::MatrixTransform::create();
    vsg::ref_ptr<vsg::Node> vsg_scene = vsg::ref_ptr<vsg::Node>(scene);
    auto builder = vsg::Builder::create();
    scene->addChild(builder->createBox());
    //scene->addChild(decorateIfRequired(builder->createBox(geomInfo, stateInfo), settings));

    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    vsg_scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 2;
    double nearFarRatio = 0.001;

    // set up the camera
    auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
    auto ellipsoidModel = vsg_scene->getRefObject<vsg::EllipsoidModel>("EllipsoidModel");

    perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio * radius, radius * 4.5);

    auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));

    // add close handler to respond to the close window button and pressing escape
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));

    // add trackball to control the Camera
    viewer->addEventHandler(vsg::Trackball::create(camera, ellipsoidModel));

    // add the CommandGraph to render the scene
    auto commandGraph = vsg::createCommandGraphForView(window, camera, vsg_scene);
    viewer->assignRecordAndSubmitTaskAndPresentation({ commandGraph });

    // compile all Vulkan objects and transfer image, vertex and primitive data to GPU
    viewer->compile();
    vsg::dmat4 mat(0.9949878,0.1,0,0,-0.1,0.994987,0,0,0,0,1,0,0,0,0,1);
    // rendering main loop
    int i = 0;
    while (viewer->advanceToNextFrame())
    {
        if (i++ > 1)
        {
        scene->matrix = scene->transform(mat);
        i = 1;
        }
        
        // pass any events into EventHandlers assigned to the Viewer
        viewer->handleEvents();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }

    // clean up done automatically thanks to ref_ptr<>
    return 0;
}
