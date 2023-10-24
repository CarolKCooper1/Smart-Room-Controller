# IoTClassroom_CNM

A Particle library for IoTClassroom_CNM

## Welcome to your library!

The IOTClassroom_CNM library is designed to be used with the CNM Ingenuity IoT Bootcamp (https://deepdivecoding.com/iot). This library contains several capabilites that are all included with IoTClassroom_CNM.h

* hue.h - control of the Phillips Hue Smart Lighting in the IoT Classroom (controlled via Phillips Hue Hub)
* wemo.h - control of the Belkin Wemo Smart Outlets in the IoT Classroom (setup for 6 classroom outlets)
* IoTTImer.h - the IoTTImer class that was created earlier the course
* Button.h - a modified version of the Button class (also earlier from the course) that includes both button pressed and button clicked (i.e., not held down).

## Usage

See the [examples](examples) folder for usage of the hue.h and wemo.h libraries

## Documentation

TODO: Describe `IoTClassroom_CNM`

## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add IoTClassroom_CNM_myname` to add the library to a project on your machine or add the IoTClassroom_CNM_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library. 

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2023 Brian Rashap

Licensed under the MIT license
