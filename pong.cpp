//Pong game
#include<iostream>
#include<stdlib.h>
#include <cstdio>
//#include<ncurses.h>
#include <termios.h>
#include<unistd.h>
#include <cstdlib>
#include <fcntl.h>
using namespace std;

enum dir {STOP=0,LEFT=1,UPLEFT=2,DOWNLEFT=3,RIGHT=4,UPRIGHT=5,DOWNRIGHT=6};
int counter=0;
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

class BufferToggle
{
    private:
        struct termios t;

    public:

        /*
         * Disables buffered input
         */

        void off(void)
        {
            tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
            t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings
        }


        /*
         * Enables buffered input
         */

        void on(void)
        {
            tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
            t.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings
        }
};

class ball{
  int x,y,OriginalX,OriginalY;
  dir direction;
public:
  ball(int x,int y){
      this->x=x;this->y=y;direction=STOP;
      OriginalX=x;OriginalY=y;
  }
  void reset(){
    x=OriginalX;
    y=OriginalY;
    direction=STOP;
  }
  int getX(){return x;}
  int getY(){return y;}
  dir getD(){return direction;}

  void randomize(){
    direction=(dir)(rand()%6+1);

  }
  void changeDIR(dir d){
    direction=d;
  }
  void move(){
    switch (direction){
      case 1:x--;break;
      case 2:x--;y--;break;
      case 3:x--;y++;break;
      case 4:x++;break;
      case 5:x++;y--;break;
      case 6:x++;y++;break;
    }
  }
  friend ostream & operator<<(ostream &o, ball b){
    o<<"BALL["<<b.x<<","<<b.x<<"]["<<b.direction<<"]"<<endl;
    return o;
  }
};

class paddle{
  int x,y,OriginalX,OriginalY;
public:
  paddle(){
    x=0;y=0;
  }
  paddle(int x,int y){
    this->x=x;this->y=y;
    OriginalX=x;OriginalY=y;
  }
  void reset(){x=OriginalX;y=OriginalY;}
  int getX(){return x;}
  int getY(){return y;}
  void up(){y--;}
  void down(){y++;}
  friend ostream & operator<<(ostream &o, paddle b){
    o<<"PADDLE["<<b.x<<","<<b.y<<"]"<<endl;
    return o;
  }

};

class gameMan{
  int width;
  int height;
  int score1,score2;
  char up1,down1,up2,down2;
  bool quit;
  ball *b;
  paddle *p1;
  paddle *p2;
public:
  gameMan(int w, int h){
    srand(time(NULL));
    quit=false;
    up1='w';up2='i';
    down1='s';down2='k';
    score1=0;score2=0;
    width=w;height=h;
    b = new ball(w/2,h/2);
    p1=new paddle(1,h/2-3);
    p2=new paddle(w-2,h/2-3);
  }
gameMan(){
    delete b , p1 , p2 ;
  }
  void scoreUp(paddle *p){
    if (p==p1) score1++;
    else score2++;
    b->reset();
    p1->reset();
    p2->reset();
  }
  void Draw(){
    system("clear");
    for(int i=0;i<width+2;i++)
      cout<<"\xB2";
    cout<<endl<<endl;
    cout<<"FLICKER PONG=============created by Another"<<endl;
    cout<<"PLAYER1:-"<<score1<<":::::::::::::::::::::::PLAYER2:-"<<score2<<endl;
    cout<<endl<<endl;
    for(int i=0;i<width+2;i++)
      cout<<"\xB2";
    cout<<endl;

    for (int i=0; i < height; i++){
      for(int j=0;j<width;j++){
        int ballx=b->getX();
        int bally=b->getY();
        int p1x=p1->getX();
        int p1y=p1->getY();
        int p2x=p2->getX();
        int p2y=p2->getY();
        if(j==0)
          cout<<"H";

        if(ballx==j&&bally==i)
          cout<<"O";
        else if (p1x==j&&p1y==i)
          cout<<"\xDB";
        else if (p1x==j&&p1y+1==i)
            cout<<"\xDB";
        else if (p1x==j&&p1y+2==i)
            cout<<"\xDB";
        else if (p1x==j&&p1y+3==i)
            cout<<"\xDB";
        else if (p2x==j&&p2y==i)
          cout<<"\xDB";
        else if (p2x==j&&p2y+1==i)
              cout<<"\xDB";
        else if (p2x==j&&p2y+2==i)
              cout<<"\xDB";
        else if (p2x==j&&p2y+3==i)
              cout<<"\xDB";
        else
          cout<<" ";

        if(j==width-1)
          cout<<"H";
      }
      cout << endl;
    }

    for(int i=0;i<width+2;i++)
      cout<<"\xB2";
    cout<<endl;

    for(int i=0;i<width+2;i++)
      cout<<"\xB2";
    cout<<endl;
    for(int i=0;i<width+2;i++)
      cout<<"\xB2";

      usleep(10000);
  }
  void Input(){
    if(counter%10==0)
    b->move();
    int ballx=b->getX();
    int bally=b->getY();
    int p1x=p1->getX();
    int p1y=p1->getY();
    int p2x=p2->getX();
    int p2y=p2->getY();
    BufferToggle bt;
    bt.off();
    if (kbhit())
    { char c=std::getchar();
      if (c==up1)
        if (p1y>0)
          p1->up();
      if (c==down1)
        if (p1y+4<height)
          p1->down();
      if (c==up2)
        if (p2y>0)
          p2->up();
      if (c==down2)
        if (p2y+4<height)
          p2->down();

      if (b->getD() == STOP)
          b->randomize();

      if (c == 'q')
          quit = true;
  }
  bt.on();
}
  void Logic()
  {
      int ballx = b->getX();
      int bally = b->getY();
      int player1x = p1->getX();
      int player2x = p2->getX();
      int player1y = p1->getY();
      int player2y = p2->getY();

      //left paddle
      for (int i = 0; i < 4; i++)
          if (ballx == player1x + 1)
              if (bally == player1y + i)
                  b->changeDIR((dir)((rand() % 3) + 4));

      //right paddle
      for (int i = 0; i < 4; i++)
          if (ballx == player2x - 1)
              if (bally == player2y + i)
                  b->changeDIR((dir)((rand() % 3) + 1));

      //bottom wall
      if (bally == height - 1)
          b->changeDIR(b->getD() == DOWNRIGHT ? UPRIGHT : UPLEFT);
      //top wall
      if (bally == 0)
          b->changeDIR(b->getD() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
      //right wall
      if (ballx == width - 1)
          scoreUp(p1);
      //left wall
      if (ballx == 0)
          scoreUp(p2);
  }
  void Run()
  {
      while (!quit)
      {
          Draw();
          Input();
          Logic();
          counter++;
          if(counter==1000)
            counter=0;
      }
  }
};

int main()
{
    gameMan   c(40, 20);
    c.Run();
    return 0;
}
