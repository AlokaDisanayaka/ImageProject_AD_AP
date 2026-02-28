//
// Created by floodd on 26/01/2026.
//

#include "Image.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

/**
 * This function creates the image object, and then loads in the image from the given filename.
 *
 * @param filename: The name of the file to load
 * /
 */
MyImage::MyImage(string filename): fileName(filename) {
    this->load(filename);
}

/**
 *  This function will reload the original image.
 */
void MyImage::reload() {
    this->load(fileName);
}

/**
 *  This function is responsible for drawing the image scaling to fit on the screen. It also draws a grey
 *  background behind the image to allow for scaling.
 */
void MyImage::draw(sf::RenderTarget &target, sf::RenderStates states)const {

    float x = 0, y = 0;
    int type = this->size.x > this->size.y? LANDSCAPE: PORTRAIT;
    float step=1;
    float startX=this->position.x, startY=this->position.y;
    float dimW = this->targetSize.x;
    float dimH = this->targetSize.y;


    if (this->size.x < this->targetSize.x && this->size.y < this->targetSize.y) {

        dimW = this->size.x;
        startY += (this->targetSize.y - this->size.y)/2.0f;
        dimH = this->size.y;
        startX += (this->targetSize.x- this->size.x)/2.0f;;
    }
    else {
        if (this->size.x / this->targetSize.x > this->size.y / this->targetSize.y) {
            step = this->size.x / this->targetSize.x;
            dimH = this->size.y/step;
            startY += (this->targetSize.y - dimH)/2;;
        }
        else {
            step = this->size.y / (float)this->targetSize.y;
            dimW = this->size.x/step;
            startX +=  (this->targetSize.x - dimW)/2;
        }
    }
    float xStep = 0, yStep=0;
    for (float i = 0; i < dimH; ++i) {
        for (float j = 0; j < dimW; ++j)
        {
            x = (int)xStep;
            y = (int)yStep;
            int pix = (y*this->size.x) + x;
            sf::VertexArray circle(sf::PrimitiveType::Points, 1);
            circle[0].color = sf::Color(this->pixels[pix].r,this->pixels[pix].g,this->pixels[pix].b);
            circle[0].position = {j+startX,i+startY};

            target.draw(circle);
            xStep+=step;
        }
        xStep = 0;
        yStep+=step;
    }
}

/**
* The draw function will scale the image to the maximum defined by this function
*
 * @param target : the target size of the image when displayed.
 */
void MyImage::setTargetSize(sf::Vector2f target) {
    this->targetSize = target;
}

/**
 *
 * @return The size of the original image defined in the PPM file.
 */
sf::Vector2f MyImage::getSize() {
    return this->size;
}

/**
 * This function opens the given ppm file and attempts to read in the data to a continuous vector of pixels defined as
 * RGB structs.
 *
 * @param filename the file to load.
 */
void MyImage::load(string filename) {
    ifstream ifs;

    string dummy;
    ifs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) { throw("Can't open input file"); }
        std::string header;
        int w, h, b;
        // Reads in the first string to check it starts with "P6" defining a ppm file. If not throws an exception as file
        // can't be read.
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        getline(ifs, dummy);
        /* Checks to see if there is a comment after the first line */
        char c;
        c = ifs.peek();
        if (c == '#') {
            getline(ifs, dummy);
        }
        // Read in the width and height of the image.
        ifs >> w >> h >> b;
        // set the size within the image object
        this->size = {(float)w,(float)h};
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];
        this->pixels.clear();// remove any existing pixel data
        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            RGB pixel;
            pixel.r = pix[0];
            pixel.g = pix[1];
            pixel.b = pix[2];
            this->pixels.push_back(pixel);
        }

        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
}
void MyImage::save()
{
    string filename = "Output/SavedImage.ppm";
    if (this->size.x == 0 || this->size.y == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->size.x << " " <<  this->size.y << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->size.x *  this->size.y; ++i) {
            r = static_cast<unsigned char>(this->pixels[i].r);
            g = static_cast<unsigned char>(this->pixels[i].g);
            b = static_cast<unsigned char>(this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

void MyImage::filterRed()
{
    cout << "Filter Red" << endl;
    for (int i=0; i<this->size.x * this->size.y; ++i)
    {
        //Removing green and blue
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }
}

void MyImage::filterGreen()
{
    cout << "Filter Green" << endl;
    for (int i=0; i<this->size.x * this->size.y; ++i)
    {
        //Removing red and blue
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }
}
void MyImage::filterBlue()
{
    cout << "Filter Blue" << endl;
    for (int i=0; i<this->size.x * this->size.y; ++i)
    {
        //Removing red and green
        this->pixels[i].r = 0;
        this->pixels[i].g = 0;
    }
}

void MyImage::greyScale()
{
    cout << "Grey Scale" << endl;

    int width =(int) this->size.x;
    int height = (int) this-> size.y;

    //looping over each pixel
    for (int i =0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            int index= (i * width) + j;

            unsigned char r= this->pixels[index].r;
            unsigned char g= this->pixels[index].g;
            unsigned char b= this->pixels[index].b;

            unsigned char grey = (unsigned char) ((r+g+b)/3);

            this->pixels[index].r = grey;
            this->pixels[index].g = grey;
            this->pixels[index].b = grey;

        }

    }
}

void MyImage::flipHorizontal()
{
    cout << "Flip Horizontal" << endl;

    int width =(int) this->size.x;
    int height = (int) this-> size.y;

    for (int i =0;i<height;i++)
    {
        //swapping pixels from right side to left side only halfway
        for (int j=0;j<width/2;j++)
        {
           int indexL= (i * width) + j;
           int indexR= (i * width) + (width-1)-j;

            //swap
            RGB temp = this->pixels[indexL];
            this->pixels[indexL] = this->pixels[indexR];
            this->pixels[indexR] = temp;
        }
    }

}
void MyImage::flipVertical()
{
    cout << "Flip Vertical" << endl;

    int width =(int) this->size.x;
    int height = (int) this-> size.y;

    //go through eacg row
    for (int j =0;j<width;j++)
    {
        //swapping pixels from top to bottom only halfway
        for (int i =0;i<height/2;i++)
        {

            int indexT = (i * width)+j;
            int indexB= (((height -1)-i) * width )+ j;

            //swap
            RGB temp = this->pixels[indexT];
            this-> pixels[indexT] = this->pixels[indexB];
            this->pixels[indexB] = temp;


        }
    }
}

void MyImage::advancedFeature1()
{
    cout << "Rotate 90 Degrees" << endl;
    int height = this->size.y;
    int width = this->size.x;
    //Swap width and height
    int newHeight = width;
    int newWidth = height;
    //New vector to store the rotated image
    vector<RGB> newPixels(newHeight*newWidth);

    //Loop through every pixel
    for (int y=0; y<height; ++y)
    {
        for (int x=0; x<width; ++x)
        {
            //Getting the index
            int oldIndex = (y*width) + x;

            int newX = (height-1) - y;
            int newY = x;

            //Create new index
            int newIndex = (newY*newWidth) + newX;
            //Moving pixels from original position into new position
            newPixels[newIndex].r = this->pixels[oldIndex].r;
            newPixels[newIndex].g = this->pixels[oldIndex].g;
            newPixels[newIndex].b = this->pixels[oldIndex].b;
        }
    }
    this->pixels = newPixels;
    this->size.y = newHeight;
    this->size.x = newWidth;
}

//using a 3 x 3 Guassian kernel
void MyImage::advancedFeature2()
{
    cout << "Gaussian Blur" << endl;
    int width = (int) this-> size.x;
    int height = (int) this-> size.y;

    //copy the original pixels to avoid using already blurred pixels
    vector <RGB> original = this->pixels;

    //Gaussian kernel
    int kernel[3][3] = {
        {1,2,1},
        {2,4,2},
        {1,2,1}
    };

    int kernelSum = 16;

    //loop through every pixel of the image
    for (int i =0;i<height;i++)
    {
          for (int j =0;j<width;j++)
          {
              int sumR=0;
              int sumG=0;
              int sumB=0;

              //apply kernel around i,j
              for (int ki =-1;ki<=1;ki++)//kernel row
              {
                  for (int kj =-1;kj<=1;kj++) //kernel column
                  {
                     int sampleI= i+ki;
                      int sampleJ= j+kj;

                      //clamp to sides to avoid going outside the image
                      sampleI = std::max(0, std::min(sampleI, height - 1)); //if below 0, make it 0
                      sampleJ = std::max(0, std::min(sampleJ, width - 1)); //if aove max , make it max

                      int weight= kernel [ki+1][kj+1];

                      //convert row,column into index
                      int sampleIndex= (sampleI* width) + sampleJ;

                      sumR= sumR+original[sampleIndex].r *weight;
                      sumG= sumG+original[sampleIndex].g *weight;
                      sumB= sumB+original[sampleIndex].b *weight;
                  }

              }

              int index =(i * width) + j;

              this->pixels[index].r = (unsigned char)(sumR/kernelSum);
              this->pixels[index].g = (unsigned char)(sumG/kernelSum);
              this->pixels[index].b = (unsigned char)(sumB/kernelSum);


          }

    }

}
void MyImage::advancedFeature3()
{


}

void MyImage::advancedFeatureExtra() {
    cout << "Cartoon Filter" << endl;

    int width =(int) this-> size.x;
    int height = (int) this-> size.y;

    //check if image exists
    if (width <=0 || height <=0||this->pixels.empty())
    {

       cout << "Image is empty! Cannot apply cartoon filter" << endl;

    }

    //+++++++++++++++ 1.Light smoothing +++++++++++++++
    // cartoon filter looks better with smothed version of the picture
    //applying a 3x3 box blur to smooth the picture for better result

    vector<RGB> smoothed = this->pixels; // start with original
    vector<RGB> original = this->pixels; // copy to read from safely


    for (int i= 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            //  the 3x3 area around i,j
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int si = i + di;
                    int sj = j + dj;

                    // Clamp to avoid going outside the image
                    si = std::max(0, std::min(si, width - 1));
                    sj = std::max(0, std::min(sj, height - 1));

                    int idx = (si * width) + sj;

                    sumR += original[idx].r;
                    sumG += original[idx].g;
                    sumB += original[idx].b;
                    count++;
                }
            }

            int index = (i * width) + j;

            // Average of neighbors
            smoothed[index].r = (unsigned char)(sumR / count);
            smoothed[index].g = (unsigned char)(sumG / count);
            smoothed[index].b = (unsigned char)(sumB / count);
        }
    }

    //Replace pixels with smoothed ones before k-means
    this->pixels = smoothed;



}
