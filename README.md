# ZERO

**Local Development Build Commands**

cmake -S . -B build

cmake --build build --config Debug

.\build\Debug\ZERO.exe

<h2 style="display:inline; margin-bottom: 2rem;">My Learning</h2>

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

**Detour: Creating Scenes**

At the time of writing, I was manually creating a cube in main by defining all of its vertices and defining each triangle that connected the vertices. This worked but it was very hard to add more cubes and easily transform/rotate them, instead it was time to abstract things a bit, to support the rendering of arbitrary objects. To this end we now consider scenes.

Suppose I want to render a bunch of different objects, how do i represent each object in a way that my renderer can calculate them all without caring about the objects itself? The idea is to create a SceneObject, a SceneObject tells information on its position and rotation, it also holds the underlying mesh. A mesh is essentially the representation of the object we are trying to render. It is primarily composed of the list of vertices, and the triangles that connect those vertices. The mesh describes the underlying shape, whereas the SceneObject dictates where the shape will be rendered in the scene and how it will be scaled. Ie a big cube and a small cube are both cubes, they share an underlying cube mesh, but their positions and transformations may be different afterwards.

With that, a scene is simply a collection of SceneObjects, all the renderer cares about is that it accepts triangles, it doesnt need to know anything else.

**Seeing through walls?**

You might've noticed by now that even with back-face culling, that we might still see some triangles that we shouldn't, what sort of case is this? Consider the following rendering of 3 cubes spaced apart, I color each triangle differently for illustration purposes

<img width="1551" height="882" alt="image" src="https://github.com/user-attachments/assets/d7c5583e-afe4-4634-95d5-29ff3504ba4c" />

Now look what happens when we put all 3 of them side by side, i.e. the middle cube should be hiding the side faces of the other cubes based on perspective.

<img width="1315" height="757" alt="image" src="https://github.com/user-attachments/assets/eb004826-c7f2-49a5-be23-8f81cb1d4de3" />

Uh oh, seems as though we are able to see through the middle cube and see the sides of the left and right cube. This is what we will work on fixing next!

Think about two triangles fighting over a pixel, it follows that whichever triangle is closer to the camera should get priority. This is the idea behind a Z buffer, essentially we will extend our framebuffer to also store the depth of the point in 3D space  being drawn at a specific pixel, it follows then that we can only draw on a pixel if the incoming depth is closer than what was previously drawn there.

How do we calculate depth? The first thing we need to learn is barycentric coordinates, essentially suppose we know a point P is inside a 2D triangle ABC, then we note that P can be represented as a linear combination of the A + u(B - A) + v(C - A).

We can expand this equation to get:

P = (1 - u - v)A + uB + vC

If we let a = (1 - u - v), b = u, c = v

Then we have that

P = aA + bB + cC

These are called barycentric coordinates. Essentially, we're representing a point inside a triangle as a weighted combination of its three vertices. One interesting property is that these weights will always add up to 1, which follows directly from how we defined them

a + b + c = (1 - u - v) + u + v = 1

The cool part is that we can use our existing edge functions to calculate the barycentric coordinates of each pixel using the three projected vertices. This tells us where the pixel lies relative to the triangle in 2D screen space. However, we can't directly use these values to get the original 3D depth. This is because perspective projection distorts distances, ie a point halfway along a 3D edge doesn't necessarily appear halfway along its projected 2D edge.

We therefore need to account for perspective when converting our 2D barycentric coordinates into their 3D equivalents. After some complicated math, we get the following depth formula:

P.z (3D) = 1 / (
a / A.z + 
b / B.z + 
c / C.z
)

Here, a, b, and c are our 2D screen-space barycentric coordinates, while A.z, B.z and C.z are the original depths of the triangle's 3D vertices. With this we can now calculate the depth of any pixel covered by a triangle without needing to reconstruct its full 3D position.

After implementing we can see it's working!

Before (drawing on top of each other):

<img width="1290" height="652" alt="image" src="https://github.com/user-attachments/assets/6beff95a-3a3c-433a-a22f-aad2f9995644" />

After (Middle cube gets depth priority):

<img width="968" height="521" alt="image" src="https://github.com/user-attachments/assets/223dacff-796b-49f6-b1a7-ff487026ac9f" />

**Another detour: Camera movement**

Currently everything has been built with a fixed camera at (0, 0, 0) facing in the positive Z direction. But obviously as people we move around and see stuff from different angles, so our camera should be able to move as well.

The first question that comes to mind, is if our camera is now in a different position, potentially facing in different directions as well, then how do we tell what objects the camera can see? Well it turns out the answer is much simpler than we thought, instead of trying to do all the calculations directly assuming the camera is in an arbitrary position, we can instead claim that the origin is wherever the camera is curretly. More specifically we mark wherever the camera is as the origin of the camera space, which is different from the original world space of our scene. Afterwards, we assume the camerea is facing in the positive Z direction in camera space. It follows then that in camera space, our camera could function the exact same way it functions currently with a fixed camera.

Now the question is, how do know where the objects will be in camera space? I.e. how do we map objects in our scene from world space to camera space? Well the answer is surprisingly simple, for raw position differences, we can just translate all world objects to match correctly in our camera space. For rotations the idea is quite similar, imagine youre facing a box, and then you turn left, one way to interpret this is you turned left from the box, another way is that the box had turned right from you. It follows then that if we use the second interpretation, then if we fix our camera is rotated however much left, then we can just keep our camera still and rotate the box however much right. This is the idea of inverse rotations. After performing all such operations, we should now have a camera space where our camera is functionally the same as our original fixed camera, and the objects in the world have been mapped accordingly. 

Note: we do not actually move the objects in world space. We make a copy of the world space and we transform the copy to achieve our camera space for rendering purposes.

After adding in some basic camera movement and rotation using delta time approach (ie speed depends on total time elapsed rather than the # of frames rendered). We are now able to move around our scene!

https://github.com/user-attachments/assets/cec2cfaf-9e16-4449-9178-f5862d79a9b1

**Near-Plane Clipping**

If you paid attention to the video, you might have noticed that whenever I got too close to the cube or when I went inside it, the screen just became a mess of triangles. This is because currently I draw all triangles whose surface is facing the camera, that includes triangles that are behind us as well (I didn't show it but if you just turn around 180 from the cubes, they will render on your screen even though you're facing away from them). 

A simple solution you may think of is to just remove all triangles who are behind us. But there is a special case worth considering. Suppose there is a triangle who is partially behind the camera, i.e. it it has a vertex in front of the camera and behind the camera. It follows we should only see part of the triangle in this case.

To this end, we introduce a near-plane, similar to our image plane, we add another plane who's very close to the camera, i.e. z = 0.1. And the idea is that anything that goes behind this plane gets clipped. Let's now consider the idea of the partially clipped triangle. It follows that if its partially clipped, it either has 1 or 2 vertices infront of the near-plane.

If it only has 1 vertex infront of the near plane, this case is simple, after clipping it at the near-plane, it will just produce a new, but smaller triangle. On the other hand, if there are 2 vertices in front of the plane, after clipping we will end up with a quadrilateral! In this case we must break it up back into triangles which our renderer supports, thankfully this case is not too bad since we cut across the diagonal. We note that the order of the vertices of the new triangles we return is important due to the winding directions we discussed earlier when performing triangle rasterization and back-face culling.

With this, we are ready to implement near-plane clipping which will solve a lot of issues!

https://github.com/user-attachments/assets/04f12a25-3d79-493b-a21e-b5fd5a3dbee7

