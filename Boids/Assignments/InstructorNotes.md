# Notes for instructors

Libby Shoop

The original paper by Craig Reynolds [[1]](#1) is quite approachable and students may find it interesting. I recommend providing a copy for them (I can't include it here for copyright reasons). He is extremely enthusiastic about modeling nature and how he did it.

The code written by Gary Flake can be found here: https://github.com/gwf/CBofN. It has many different examples. The boids code contained several global array variables. TODO: more here about when they helped and when they did not (check with Ethan)

## A stipulation: need for X graphics
Flake's code included files to enable a graphical X window display of the boids as arrows moving around the screen. To run this version of the code from a linux server in our department, I have instructions for my students about how to use either XQuartz on a Mac and use ssh -X to connect, or from Windows I suggest that they use MobaXterm to connect and run the code over ssh. There are certainly other ways to accomplish this, such as running Windows subsystem for linux on your own Windows laptop, or providing linux lab machines.

# Information from Flake's book

I have included an excerpt of chapter 16.3 from Flake's book as a pdf file, which I was able to download from an EBSCO electronic copy from my library. I believe under fair use copyright I am able to use this small excerpt for courses at my institution. If you are able to get this yourself, I would encourage it so that we properly follow copyright laws.

In addition, here is information is taken from Gary Flake's book about the various options of the program:

    "The boids program has many options, but most of them should be familiar by now. There are four options used to specify the radius in which the four boid rules are active, and four corresponding weighting factors. The angle option is used to specify the number of viewing degrees that the boids possess. The boids can see only other bolds that fall within a cone of the specified number of degrees in front of them. The vangle option has a similar meaning, but is used only for the view rule. The dt and ddt options specify time increment and momentum factors, and minv is used to specify the minimum speed that the boids can fly.

    For display purposes, len can be used to specify the length of the boids in pixels. And finally, psdump is used to tell the program to emit a PostScript image of
    the boids at the end of the simulation.

    A boid is displayed as a flying arrow. The head of the arrow defines an arc that swings from the left edge, to the front, and then to the right edge of the head. This arc is the same as the viewing angle; thus, if you set the viewing angle to something large, say 320 degrees, then the head will make a very sharp point. With a viewing  angle of 180 degrees, the head will be a flat line."



## References
<a id="1">[1]</a>
Reynolds, Craig W. (1987)
Flocks, herds and schools: A distributed behavioral model.
Proceedings of the 14th annual conference on Computer graphics and interactive techniques, 25--34.
