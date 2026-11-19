# ZERO

cmake -S . -B build

cmake --build build --config Debug

<details>
<summary>My Learning</summary>

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

How do we start seeing 3D objects? Well how do we see stuff in real life? Through our eyes, or through a camera if you're a computer. Understanding the camera was pretty cool, basically in a real camera, all the light rays funnel through a tiny pinhole and reflect off the inside of the camera to create an inverted image on the image plane. Instead of doing all that just to flip it at the end, computer graphics just decides to move the image plane in front of the camera hole. So instead you need to answer the question, for every edge connecting the camera hole and a 3D point I want to render, where does it intersect within the image plane?

For now I decided to keep things simple by creating a fixed camera at the origin, this made the math a lot easier. After computing the intersection points we then had to map the image plane to our actual viewport. This is basic recentering since the image plane was centered at (0, 0) where as my viewport had (0, 0) at the top left.

All that was left was to test whether I could see my test cube,

</details>
