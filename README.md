# ZERO

<details>
<summary>My Learning</summary>

**First Steps**

To begin I had to set up a basic window in SDL2 which was pretty simple. 

After that I was introduced to the concept of a framebuffer, pretty much it's a 1D array of pixels that represents the space you want to draw on, which in my case was the entire window.

After finishing my basic framebuffer, I then had to dump its information into SDL2's API to draw pixels on the screen. I.e. the framebuffer stores the pixel information, SDL2 just draws the pixels.

The first challenge was drawing a triangle... The idea is if I'm given the 3 points of my triangle, how do I know which points lie inside? It turns out there's a neat 2D cross product trick that can be used to figure out which side of a line a point lies on. Using this on the three edges of the triangle you can figure out whether or not a point lies inside or outside the triangle. There's some extra checks you need to do concerning direction semantics but I will not get too into detail on that.

All that's important is that triangles have special properties in both 2D and 3D space that other N-gon shapes do not have which make them the optimal choice when rendering. As a thought experiment, suppose you're given the N vertices of a N-gon shape, does the order in which you connect the vertices matter if your end goal is to redraw the original shape? How many other vertices at max can one vertex connect to?

<img width="1592" height="926" alt="image" src="https://github.com/user-attachments/assets/4a7588c4-8bf4-40f9-8501-25a148730319" />

</details>
