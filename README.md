# ceng477-OpenGL

##### Authors: Doruk Coşkun (/Doruk-Coskun) & Çağrı Eser (/cagries)

![An example](https://raw.githubusercontent.com/Doruk-Coskun/ceng477-OpenGL/add-phong-shading/Homework3/output-images/earth-output.png?token=AYU2WyhJN0CHLsAhLo9M9gcyLo6hFi2mks5cbDRVwA%3D%3D)

![Another example](https://raw.githubusercontent.com/Doruk-Coskun/ceng477-OpenGL/add-phong-shading/Homework3/output-images/turkey-output.png?token=AYU2W2i6D7Accxmnbu5dluSbOQBxdx29ks5cbDRqwA%3D%3D)

OpenGL project for CENG477 Computer Graphics lecture.

We have implemented an OpenGL program which renders the given scene (A physical map) using vertex and fragment shaders. Texture image (the physical map) is used as a height map. Program processes keyboard inputs to fly over the map and change the height factor.

Check `Homework3.pdf` for more implementation details.

# How to Run

Download the repository by using:

```
$ git clone https://github.com/Doruk-Coskun/ceng477-OpenGL
```

To build the project, run the following commands from the project root directory:

```
$ cd Homework3
$ make
```

This creates the `main` executable inside the same directory.

`earth.jpg` and `turkey.jpg` can be used as input files. You can run the OpenGL program by:

```
$ ./main <texture.jpg>
```

A new OpenGL window is created. 

### Controls

W, A, S, D for movement. Increase the flight speed (initial is 0) by U and decrease the flight speed by J. Height factor can be changed using O and L.
