# Boids Assignment

Libby Shoop

The following information and more tips for instructors can be found in the file called *InstructorNotes.md* in the **Assignments** folder.

## Assignment file names

The suggested starting points for assignments are:

1. *SystemsCourseBoids.md* -- this is intended for students in core courses who are studying C/C++ or have some experience with C/C++. I called it SystemsCourse because that is where I would use it in our curriculum, but other contexts may work for you. Corresponding code is in the **OMPpsPlot** folder. A sequential version with Makefile is provided as a starting point.

2. *PDCCourseOpenACC-X.md* -- this is intended for an advanced course where students have practiced OpenMP and are studying OpenACC. I labeled it PDCCourse because that s where I have used it. It emphasizes the OpenACC additions that can be made to run on the GPU, while keeping the original X display code from FLake. Corresponding code is in the **GPUpsPlot** folder. A sequential version  with Makefile is provided as a starting point; two new versions can be made for: OpenMP and OpenACC.

3. *PDCCourse-TSGL.md* -- like *PDCCourseOpenACC-X.md*, this is intended for an advanced course where students are studying OpenACC, but also requires that they obtain or have access to an installation of TSGL. In this case, some familiarity with C++ is needed. Corresponding code is in the **tsgl** folder. Three different versions of the program can be developed from the original sequential version -- the Makefile has provisions for building three different versions for: OpenMP, OpenACC multicore, OpenACC GPU.

For all of these assignments, the code for displaying the boids can be a black box for the students. The concept is to concentrate on the code for updating the position of each boid at each time step and how that can be parallelized.

## Necessary Background

The following information is included in each assignment document. If you have other OpenMP materials, you could add yours.

You need to have studied some examples from OpenMP to be able to apply them to this situation. One reference is the [PDC for Beginners book, chapter 1](https://www.learnpdc.org/PDCBeginners2e/1-sharedMemory/toctree.html), where you can see some OpenMP examples in action.

Another is the full set of pattern examples in [Chapter 2 of the Intermediate PDC book](https://www.learnpdc.org/IntermediatePDC/index.html).

To consider the scalability of your parallel solution, you might want to read about how this is determined in PDC programs by reading [PDC for Beginners book, chapter 0, section 3](https://www.learnpdc.org/PDCBeginners2e/0-introduction/3.performance.html).

To consider the scalability of your parallel solution, you might want to read about how this is determined in PDC programs by reading [PDC for Beginners book, chapter 0, section 3](https://www.learnpdc.org/PDCBeginners2e/0-introduction/3.performance.html).

To complete an OpenACC version for the GPU, you should study and practice the code examples in Chapters 7 and 8 of [the Intermediate PDC book](https://www.learnpdc.org/IntermediatePDC/index.html). It is also helpful to consider the CUDA GPU Programming model described in [Chapter 4, section 2 of the PDC for Beginners book](https://www.learnpdc.org/PDCBeginners2e/4-cuda/2-cuda-model-dim3.html). The OpenACC compiler creates CUDA code, and the programming model of 1 thread per array data element computation applies here.