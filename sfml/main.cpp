#include <SFML/Graphics.hpp>
#include <math.h>
#include <list>
#include <iostream>
#include<sstream>
#include<ctime>
#include <vector>
#include <fstream>
#include<cstdlib>

using namespace sf;

const int w=1200;
const int h=800;

float DEGTORAD= 0.017453f;

class animation
{
    public:
        float Frame, speed;
        Sprite sprite;
        std::vector<IntRect> frames;

       
        animation(){}

        animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
        {
            Frame=0;
            speed=Speed;

            for(int i=0; i<count;i++)
            {
                frames.push_back(IntRect(x+i*w, y,w, h));
            }
                sprite.setTexture(t);
                sprite.setOrigin(w/2,h/2);
                sprite.setTextureRect(frames[0]);
            

          
        }
          void update()
            {
                Frame +=speed;
                int n=frames.size();
                if(Frame>=n) Frame -=n;
                if(n>0) sprite.setTextureRect(frames[int(Frame)]);
            }

            bool isEnd()
            {
                return Frame+speed>=frames.size();      
            }
};



class entity
{
    public:
        float x,y, dx,dy,R,angle;
        bool life;
        std::string name;
        animation anim;

        entity(){life=1;}

        void settings(animation &a, int X, int Y, float Angle=0, int radius=1)
        {
            x=X; y=Y;anim=a;
            angle=Angle; R=radius;
        }
        virtual void update(){};

        void draw(RenderWindow &app)
        {
            anim.sprite.setPosition(x,y);
            anim.sprite.setRotation(angle+90);
            app.draw(anim.sprite);

           
        }

        ///
        virtual ~entity(){};
};

class asteroid:public entity
{
    public:
    asteroid()
    {
        dx=rand()%8-2;
        dy=rand()%8-2;
        name="asteroid";

    }

    void update()
    {
        x+=dx;
        y+=dy;
        if(x>w)
            x=0;
        if(x<0)
            x=w;
        if(y>h)
            y=0;
        if(y<0)
            y=h;

    }
};

class bullet: public entity
{
    public:
    bullet()
    {
        name="bullet";
    }
    void update()
    {
        dx=cos(angle*DEGTORAD)*6;
        dy=sin(angle*DEGTORAD)*6;
        x+=dx;
        y+=dy;
        if(x>w || x<0 || y>h || y<0) 
            life=0;

    }
};

class player: public entity
{
    public:
   bool thrust;

   player()
   {
     name="player";
   }

   void update()
   {
     if (thrust)
      { dx+=cos(angle*DEGTORAD)*0.2;
        dy+=sin(angle*DEGTORAD)*0.2; }
     else
      { dx*=0.99;
        dy*=0.99; }

    int maxSpeed=15;
    float speed = sqrt(dx*dx+dy*dy);
    if (speed>maxSpeed)
     { dx *= maxSpeed/speed;
       dy *= maxSpeed/speed; }

    x+=dx;
    y+=dy;

    if (x>w) x=0; if (x<0) x=w;
    if (y>h) y=0; if (y<0) y=h;
   }
};

bool isCollide(entity*a, entity*b)
{
    return(b->x -a->x)*(b->x - a->x)+(b->y -a->y)*(b->y -a->y)<(a->R + b->R)*(a->R + b->R);
}



void game(bool mouse)  
{

    RenderWindow app(VideoMode(w, h), "Asteroidi");
    app.setFramerateLimit(60);
    app.setKeyRepeatEnabled(false);
    

    Texture t1, t2,t3, t4, t5;
    t1.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\pucac.png");
    t2.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\pozadina.png");
   
    t4.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\asteroid.png");
    t5.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\metak.png");

  

    animation sBullet(t5, 0,0, 20, 20, 4, 0.8);

    Sprite sPlayer(t1), sBackground(t2), sEksplozija(t3);
    sPlayer.setTextureRect(IntRect(0, 0, 25, 40));
    
    sPlayer.setOrigin(0, 13);

    /////
   std::vector<int> hits;

    float Frame=0;
    float animSpeed=0.4;
    int frameCount =20;
    int score=0;
    int counter = 0;
    int count=0;

    animation sAsteroid(t4, 0,0, 80, 80, 4, 0.2);
    

    std::list<entity*> entities;


    for(int i=0;i<15;i++)
    {
            asteroid *a=new asteroid();
            a->settings(sAsteroid, rand()%w, rand()%h, rand()%360, 25);
            entities.push_back(a);
    }

    float x=600, y=400;
    float dx=0, dy=0, angle=0;
    bool thrust;
    bool pressed = false;


    
   /* Clock clock;
    int countdown=12;
   
    //text.setCharacterSize(24);
    std::string countdownString;
    std::ostringstream convert;*/
   /* convert<<countdown;
    countdownString=convert.str();
*/
    /*Text timerText;
    timerText.setString(countdownString);
    timerText.setPosition(50, 50);
    timerText.setCharacterSize(40);
    timerText.setFillColor(Color::White);*/


    while(app.isOpen())
    
    {  
        int count=0;
        int hit;
        counter++;
        if (counter > 170)
        {
            counter = 0;
            asteroid *a=new asteroid();
            a->settings(sAsteroid, rand()%w, rand()%h, rand()%360, 25);
            entities.push_back(a);
        }
           // app.draw(sPlayer);
            Event event;
            while(app.pollEvent(event))     //za provjeru novih evenata
            {
               if(event.type== Event::Closed )
                app.close();

                if(event.type== Event::MouseButtonPressed and mouse)        //ako je mouse odabran 
                    if(event.key.code== Mouse::Left)
                    {
                        bullet*b=new bullet();
                        b->settings(sBullet, x, y, angle, 10);
                        entities.push_back(b);
                    }
                    
                
            }
            //// 
            //hits[count++]=hit;
           // count++;
          /*  hits.push_back(hit);


        int timer=clock.getElapsedTime().asSeconds();
        std::cout<<timer<<std::endl;

        if(timer>0){
            countdown--;

        //    timerText.setString(std:: to_string(countdown));            //////////???????ne  radi

            //clock.restart();
        }*/
        /*
        if(timer=0)
        app.close();
     */

        if(Keyboard::isKeyPressed(Keyboard::Space)and not mouse and not pressed)    //not pressed da ne moze pucat dok se drzi dugo
        {                                                                           //ako drzim onda nije not pressed pa preskoci petlju
            bullet*b=new bullet();
            b->settings(sBullet, x, y, angle, 10);
            entities.push_back(b);
            pressed = true;
        }
        if(not Keyboard::isKeyPressed(Keyboard::Space))
        {
            pressed = false;
        }
        if(Keyboard::isKeyPressed(Keyboard::Right)) angle+=3;
        if(Keyboard::isKeyPressed(Keyboard::Left)) angle-=3;
        if(Keyboard::isKeyPressed(Keyboard::Up)) thrust=true;
        else thrust=false;

        for(auto a:entities)
           { for(auto b:entities)
                if(a->name=="asteroid"&& b->name=="bullet")     //usporeduje i gleda jel jedan asteroid i jedan metak
                    if(isCollide(a,b))          //gleda jesu se sudarili
                    {
                        a->life=false;
                        b->life=false;
                        score+=1;
                     }
             }
     //   player*p;
     //p->settings(sPlayer, x, y, angle, 20);
        //entities.push_back(p);
      /*  for(auto a:entities)
           { for(auto p:entities)
                if(a->name=="asteroid"&& p->name=="player")
                    if(collision(a,p) == 0)
                    {
                        app.close();
                     }
             }*/


        ///pokreti pucača///

        if(thrust)
        {
            dx+=cos(angle*DEGTORAD)*0.2;
            dy+=sin(angle*DEGTORAD)*0.2;
        }
        else
        {
            dx*=0.99;
            dy*=0.99;
        }

        int maxSpeed=15;
        float speed=sqrt(dx*dx+dy*dy);
        if(speed>maxSpeed)
        {
            dx*=maxSpeed/speed;         //usporava ga ako je brzina veca od 15
            dy*=maxSpeed/speed;     ///pikseli po šezdesetini sekunde
        }
        x+=dx;
        y+=dy;

        if(x>w)
            x=0;
        if(x<0) 
            x=w;
        if(y>h)
            y=0;
        if(y<0)
            y=h;

        sPlayer.setPosition(x,y);       //kad se pomakne premjesti ga
        sPlayer.setRotation(angle+90);

        for(auto i=entities.begin(); i!=entities.end();)
        {
            entity *e=*i;
            e->update();
            e->anim.update();

            if(e->life==false) 
            {
                i=entities.erase(i); delete e;
                
            }
            else i++;
        }

        ////
        //print_vector(hits);

        sAsteroid.update();

        app.clear();
        app.draw(sBackground);
        app.draw(sPlayer);
      //  app.draw(timerText);
        //app.draw(sAsteroid.sprite);
        for(auto i:entities) i->draw(app);

       // print_vector(hits);

        //LAST SCORE

        std::ifstream input("last_score.txt");
        int last_score;
        input>>last_score;

        std::ofstream output("last_score.txt");
        output<< score;
        ///

        app.display();
    }

}

void gameMenu()
{
    int i = 0;
    Texture play, mouse, space;
    play.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\play.png");
    space.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\space.png");
    mouse.loadFromFile("C:\\Users\\andre\\OneDrive\\Documents\\asteroidi\\mouse.png");

    Sprite button1(play), button2(space), button3(mouse);
    button1.setTextureRect(IntRect(0, 0, 330, 130));
    button1.setPosition(400, 200);

    button2.setTextureRect(IntRect(0,0,330, 130));
    button2.setPosition(400, 400);

    button3.setPosition(400,400);
    


    RenderWindow app(VideoMode(w, h), "Asteroidi");
    app.setFramerateLimit(60);

     while(app.isOpen())
        {
           // app.draw(sPlayer);
            Event event;


            while(app.pollEvent(event))
            {
               if(event.type== Event::Closed)
                app.close();

                if(event.type== Event::MouseButtonPressed)
                    if(event.key.code== Mouse::Left)
                    {
                        sf::Vector2i position = sf::Mouse::getPosition(app);
                        if (position.x>= 400 and position.y >= 200 and position.x<= 730 and position.y <= 330)
                        {
                            game(i%2);      //provjerava jel parni il neparni tj jel mis il space
                        }
                        else if(position.x>=400 and position.y>=400 and position.x<=730 and position.y<=530)
                        {
                            i++;        ///ako klinknem drugu opciju, i se poveca, ide parni pa neparni
                        }

                    }
                    
            }

            app.draw(button1);
            if(i%2 == 0)
            app.draw(button2);
            else
            app.draw(button3);
            app.display(); 
        }
        
}

/// dijalog
/*
int dijalog(Event event)
{
    String playerInput;
    Text playerText;
    RenderWindow app(VideoMode(200, 200), "upisi ime");
    app.setFramerateLimit(60);

    if (event.type== Event:: TextEntered)
    {
        playerInput +=event.text.unicode;
        playerText.setString(playerInput);
    }

    app.draw(playerText);

}
*/
int main()
{
    gameMenu();

    return 0;
}