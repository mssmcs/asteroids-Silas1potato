#include "graphics.h"

using namespace std;
using namespace mssm; //i hate coding with a buring passion
class Ship {
public:
    void draw(Graphics& g);
    void Update(Graphics& g);
    Vec2d position;
    double scale;
    double angle;
    Vec2d velocity;
    Vec2d a;
    double noDieTime;
    int Lives;
    int points;
};

class Asteroid {
public:
    void draw(Graphics& g);
    void update(Graphics& g);
    Vec2d position;
    Vec2d velocity;
    double angle;
    double angularVelo;
    double scale;
};
class Bullet {
public:
    void draw(Graphics &g);
    void update(Graphics &g);
    Vec2d position;
    Vec2d velocity;
    double Life;
};

void UrDead(Graphics &g)
{
    while (g.draw()) {
        if(!g.isDrawable()) {
            continue;
        }

            g.text({200,200}, 60, "YOU DIED", RED);
            g.text({200,400}, 60, "GOOD LUCK NEXT TIME!", RED);

        }
        if(g.onKeyPress(Key::ENTER)) {
            return;
        }

}

void Bullet::draw(Graphics &g)
{
    g.ellipse(position, 10, 5, GREEN, GREEN);
}

void Bullet::update(Graphics& g)
{
    Life--;

    position.x += velocity.x;

    position.y += velocity.y;

    if (position.x < -20) {
        position.x = g.width() -20;

    }

    if (position.y < -20) {
        position.y = g.height()-20;
    }

    if (position.x > g.width()-20) {
        position.x = -20;
    }

    if (position.y > g.height()-20) {
        position.y = -20;
    }
}

void Asteroid::update(Graphics& g)
{


    angle += angularVelo;

    position.x += velocity.x;

    position.y += velocity.y;

    if (position.x < -20) {
        position.x = g.width() -20;

    }

    if (position.y < -20) {
        position.y = g.height()-20;
    }

    if (position.x > g.width()-20) {
        position.x = -20;
    }

    if (position.y > g.height()-20) {
        position.y = -20;
    }

}


void Asteroid::draw(Graphics &g)
{

    Array<Vec2d> pts1{ {-5, -25}, {-15, -10}, {-20, 5}, {-5, 20}, {25, 20}, {20, -10} };

    for (int i = 0; i < pts1.size(); i++) {
        pts1[i].rotate(angle);
        pts1[i] *= scale;
        pts1[i] += position;
    }

    g.polygon(pts1, WHITE, GREY);
}

void Ship::draw(Graphics& g) {

    Array<Vec2d>pts{  {0,21}, {-12,30}, {0,0}, {12, 30} };

    for(int i = 0; i < pts.size(); i++) {
        pts[i].rotate(angle+1.57);
        pts[i] += position;

    }
    Color x = RED;

    if(noDieTime > 0) {
        x = GREEN;
    }
    g.polygon(pts, GREY, x);

}

void Ship::Update(Graphics& g)  {

    if(Lives <= 0) {
        UrDead(g);
    }


    noDieTime -= 3;

    position.x += velocity.x;

    position.y += velocity.y;

    a.x = 0.01 * cos(angle);

    a.y = sin(angle)* 0.015;

    if(g.isKeyPressed(Key::Up)) {
        velocity = velocity + a;
    }
    else{
        velocity *= 0.99;
    }
    if(g.isKeyPressed(Key::Right)) {
        angle += 0.05;
    }
    if(g.isKeyPressed(Key::Left)) {
        angle -= 0.05;
    }
    if (position.x < -20) {
        position.x = g.width() -20;
    }

    if (position.y < -20) {
        position.y = g.height()-20;
    }

    if (position.x > g.width()-20) {
        position.x = -20;
    }

    if (position.y > g.height()-20) {
        position.y = -20;
    }
}
int main()
{
    Graphics g("Testing", 1024, 768);

    Ship play;

    play.position.y = 300;

    play.position.x = 300;

    Array<Asteroid>asteroids;

    for(int i = 0; i < 5; i++) {

        Asteroid Bob;

        Bob.position = {randomInt(1, 1024), randomInt(1, 768)};

        Bob.velocity = {randomDouble(-4,4),randomDouble(-4,4)};

        Bob.angularVelo = randomDouble(0.009, 0.03);

        Bob.scale = 3;

        Bob.angle = 0.5;

        asteroids.append(Bob);
    }

    Array<Bullet>bullets;

    Array<Vec2d>stars;

    stars.resize(100);

    // for(int a = 0; a < 100; a++) {
    //     stars.append(randomInt(1,1024), randomInt(1,768));
    // }

    play.Lives = 4;

    play.points = 0;

    while (g.draw()) {
        if (!g.isDrawable()) {
            continue;
        }

        if(asteroids.size() == 0) {

            for(int i = 0; i < 5; i++) {

                Asteroid Bob;

                Bob.position = {randomInt(1, 1024), randomInt(1, 768)};

                Bob.velocity = {randomDouble(-4,4),randomDouble(-4,4)};

                Bob.angularVelo = randomDouble(0.009, 0.03);

                Bob.scale = 3;

                Bob.angle = 0.5;

                asteroids.append(Bob);
            }
        }

        g.text({175,100}, 50, to_string(play.Lives), RED);
        g.text({0,100}, 50, "Lives: ", RED);
        g.text({175,50}, 50, to_string(play.points), RED);
        g.text({0,50}, 50, "Points: ", RED);

        for(int i = 0; i < asteroids.size(); i++) {
            double x = (asteroids[i].position.x - play.position.x);
            double y = (asteroids[i].position.y - play.position.y);
            double d = (sqrt((x*x) + (y*y)));
            if(d <= asteroids[i].scale*26 && play.noDieTime <= 0) {
                play.position.x = 500;
                play.position.x = 350;
                play.velocity.x = 0;
                play.velocity.y = 0;
                play.noDieTime = 600;
                play.Lives = play.Lives - 1;
            }

        }

        play.draw(g);
        play.Update(g);

        for(int i = 0; i < asteroids.size(); i++) {
            asteroids[i].update(g);
            asteroids[i].draw(g);

        }
        if(g.onKeyPress(Key::Space)) {
            Bullet a;

            a.Life = 150;

            a.position = play.position;

            a.velocity.x = 5 * cos(play.angle);

            a.velocity.y = sin(play.angle)* 5;

            bullets.append(a);
        }

        for(int i = 0; i < bullets.size(); i++) {

            bullets[i].draw(g);
            bullets[i].update(g);

            if(bullets[i].Life <= 0) {
                bullets.removeAtIndex(i);
            }
        }
        for(int z = 0; z < asteroids.size(); ++z) {
            for(int i = 0; i < bullets.size(); i++) {
                double s = (asteroids[z].position.x - bullets[i].position.x);
                double r = (asteroids[z].position.y - bullets[i].position.y);
                double t = (sqrt((s*s) + (r*r)));
                if(t <= 50) {
                    asteroids[z].scale--;
                    bullets.removeAtIndex(i);
                    play.points += 50;
                    if(asteroids[z].scale == 0) {
                        asteroids.removeAtIndex(z);

                    }
                    else {
                        Asteroid Bob;

                        Bob.position = {asteroids[z].position};

                        Bob.velocity = {randomDouble(-4,4),randomDouble(-4,4)};

                        Bob.angularVelo = randomDouble(0.009, 0.03);

                        Bob.scale = asteroids[z].scale;

                        Bob.angle = 0.5;

                        asteroids.append(Bob);
                    }
                    break;
                }
            }
        }

        g.points(stars);




    }

    return 0;
}




