# ZERO

**Local Development Build Commands**

cmake -S . -B build

cmake --build build --config Debug

.\build\Debug\ZERO.exe

<details>
<summary style="margin-bottom: 2rem;"> 
  <h2 style="display:inline;">My Learning</h2>
</summary>

**First Steps**

To begin I had to set up a basic window in SDL2 which was pretty simple. 

After that I was introduced to the concept of a framebuffer, pretty much it's a 1D array of pixels that represents the space you want to draw on, which in my case was the entire window.

After finishing my basic framebuffer, I then had to dump its information into SDL2's API to draw pixels on the screen. I.e. the framebuffer stores the pixel information, SDL2 just draws the pixels.

The first challenge was drawing a triangle... The idea is if I'm given the 3 points of my triangle, how do I know which points lie inside? It turns out there's a neat 2D cross product trick that can be used to figure out which side of a line a point lies on. Using this on the three edges of the triangle you can figure out whether or not a point lies inside or outside the triangle. There's some extra checks you need to do concerning direction semantics but I will not get too into detail on that.

All that's important is that triangles have special properties in both 2D and 3D space that other N-gon shapes do not have which make them the optimal choice when rendering. As a thought experiment, suppose you're given the N vertices of a N-gon shape, does the order in which you connect the vertices matter if your end goal is to redraw the original shape? How many other vertices at max can one vertex connect to?

<img width="1592" height="926" alt="image" src="https://github.com/user-attachments/assets/4a7588c4-8bf4-40f9-8501-25a148730319" />

**Moving to 3D**

With the 2D triangle out of the way, I started trying to bridge the gap between 2D and 3D.

The first thing I had to set up was the concept of 3D space, to do this I built out a basic Vec3 class which can represent both 3D points and 3D vectors. Afterwards I created a mini cube object in main just for testing purposes.

How do we start seeing 3D objects? Well how do we see stuff in real life? Through our eyes, or through a camera if you're a computer. Understanding the camera was pretty cool, basically in a real camera, all the light rays funnel through a tiny pinhole and reflect off the inside of the camera to create an inverted image on the image plane. Instead of doing all that just to flip it at the end, computer graphics just decides to move the image plane in front of the camera hole which produces an upright image. So instead you need to answer the question, for every edge connecting the camera hole and a 3D point I want to render, where does it intersect within the image plane?

For now I decided to keep things simple by creating a fixed camera at the origin, this made the math a lot easier. After computing the intersection points we then had to map the image plane to our actual viewport. This is basic recentering since the image plane was centered at (0, 0) where as my viewport had (0, 0) at the top left.

All that was left was to test whether I could see my test cube,

<img width="1581" height="916" alt="image" src="https://github.com/user-attachments/assets/8f4e0145-5b01-466a-b299-9b8b5dcbbc2d" />

**Visibility**

While it's not obvious in the image above, we are currently drawing all sides of the triangle, even though in practice, you wouldn't be able to see the back of the cube. This leads us to some insights on how we can optimize our rendering and how we can avoid drawing some triangles entirely.

We now discuss back-face culling, imagine a closed object like a cube. If you think about the surface of each face, consider a vector pointing outwards from each face of the cube. This is kinda like the diretion light will reflect off the surface. It follows that if the light is reflecting in the opposite direction of our camera, it will never reach, so we shouldn't render faces of the cube that are back facing.

Note: The light analogy is not entirely correct since normal vectors depend purely on the ordering of vertices, but I find it helpful to reason about

How do we tell if a face is facing the general direction of our camera? We begin by taking the cross product of AB and AC in our triangle ABC. This will give a perpendicular vector which we will consider as the vector pointing out of the surface. Note that this depends on the ordering vertices ABC, correct 3D model formats will ensure that the triangles provided follow a correct convention so that when we take cross products, the vector returned is pointing in the direction of the surface.

Afterwards we then compare this normal vector to the vector from the triangle to our camera by taking their dot product. If they are similar facing, ie difference in their angle is less than 90 degrees, then we say it is facing our camera, otherwise it is generally pointing away from our camera and we can skip drawing it.

After implementing our basic back-face culling, my cube which is comprised of 12 triangles only renders 4 triangles and culls the other 8!

Sample code snippet at the time of testing:

```
    int culled = 0;
    for (int i = 0; i < 12; i++) {
        Vec3 a = vertices[triangles[i][0]];
        Vec3 b = vertices[triangles[i][1]];
        Vec3 c = vertices[triangles[i][2]];

        if (isBackFacing(a, b, c, cameraPosition)) {
            culled++;
            continue;
        }

        Vec2 p1_projected = camera.project(a, window_width, window_height);
        Vec2 p2_projected = camera.project(b, window_width, window_height);
        Vec2 p3_projected = camera.project(c, window_width, window_height);

        framebuffer.drawTriangle(p1_projected, p2_projected, p3_projected, red);
    }

    std::cout << "Culled triangles: " << culled << '\n';
```

The output:

```
Culled triangles: 8
```

</details>
