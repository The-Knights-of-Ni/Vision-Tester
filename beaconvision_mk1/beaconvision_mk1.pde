PImage pic;
final int buf = 50; //buffer zone around the edges of the image

void setup() {
  size(600, 400); 
  pic = loadImage("004.jpg");
  noLoop();
}

void draw() {
  pic.resize(600,400);
  loadPixels();
  int dim = pic.width*pic.height; //dimension of the picture
  //check the color of every pixel in the frame
  for (int i = 0; i < dim; i++) {    
    //check if the pixel is red
    if (red(pic.pixels[i]) > 235 && blue(pic.pixels[i]) < 235) {
      pic.pixels[i] = color(255,0,0);
    }
    //check if the pixel is blue 
    else if (blue(pic.pixels[i]) > 235 && red(pic.pixels[i]) < 235) {
      pic.pixels[i] = color(0, 0, 255);
    }
    //make the pixel black if it is not red or blue
    else {
      pic.pixels[i] = color(0, 0, 0);
    }
  }
  
  //eliminate random outlying non-black pixels
  for(int i=0; i<dim; i++) 
  {
    //make the right and left edges all black
    if(i%pic.width < buf || i%pic.width > pic.width-buf)
      pic.pixels[i] = color(0,0,0);
    //make the top and bottom pixels black
    else if(i<pic.width*buf || i>dim-pic.width*buf)
      pic.pixels[i] = color(0,0,0);
    //make loner pixels black
    else if(pic.pixels[i+10] ==  color(0,0,0) && pic.pixels[i-10] ==  color(0,0,0) || pic.pixels[i+pic.width*10] ==  color(0,0,0) && pic.pixels[i-pic.width*10] ==  color(0,0,0))
      pic.pixels[i] = color(0,0,0);
  }
  //first and last pixels of each color
  int firstBlue=pic.width-1;
  int firstRed=pic.width-1;
  int lastBlue=0;
  int lastRed=0;
  int midBlue = 0;
  int midRed = 0;
  
  //find the blue pixel farthest left
  for(int i=0; i<dim; i++) {
     if(pic.pixels[i] == color(0,0,255) && i%pic.width <= firstBlue%pic.width) {
        firstBlue = i;
     }
  }
  //find the blue pixel farthest right
  for(int i=dim-1; i>0; i--) {
     if(pic.pixels[i] == color(0,0,255) && i%pic.width >= lastBlue%pic.width) {
        lastBlue = i;
     }
  }

  //find the centerline of the blue pixels
  midBlue = (lastBlue%pic.width+firstBlue%pic.width)/2;
  
  //highlight the line of the middle of the blue
  for(int i=0; i<dim; i++){
     if(i%pic.width==midBlue)
       pic.pixels[i]=color(255,255,0);
  }

  //find the red pixel farthest left
  for(int i=0; i<dim; i++) {
     if(pic.pixels[i] == color(255,0,0) && i%pic.width <= firstRed%pic.width) {
        firstRed = i;
     }
  }
  //find the red pixel farthest right
  for(int i=dim-1; i>0; i--) {
     if(pic.pixels[i] == color(255,0,0) && i%pic.width >= lastRed%pic.width) {
        lastRed = i;
     }
  }

  //find the centerline of the red pixels
  midRed = (lastRed%pic.width+firstRed%pic.width)/2;
  
  //highlight the line of the middle of the red
  for(int i=0; i<dim; i++){
     if(i%pic.width==midRed)
       pic.pixels[i]=color(255,255,255);
  }
  if(midRed>midBlue)
    println("Red is Right, Blue is Left");
  else
    println("Blue Right, Red is Left");
  
  updatePixels();
  image(pic, 0, 0);
}