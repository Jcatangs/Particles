#include "Particle.h"
#include <random>

int switch_c = 0;


bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }


    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{
    random_device rand;
    mt19937 gen(rand());
    uniform_real_distribution<float> urd_theta(0.0, M_PI / 2.0); //TRY TRY TRY TRY TRY using mt19937
    uniform_real_distribution<float> urd_pixel_V(100.0, 500.0);

    m_ttl = TTL; //float 
    m_numPoints = numPoints; //int
    m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI; //float 

    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);

    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane); //Vector2f

    float m_vx_input = urd_pixel_V(gen);
    if (rand() % 2 != 0) m_vx_input *= -1;
    m_vx = m_vx_input; //float 
    m_vy = urd_pixel_V(gen); //float 500 500 500 ???
    
    m_color1 = Color::Magenta; //Color 
    m_color2 = Color::Cyan; //Color 
    
    float theta = urd_theta(gen);
    float dTheta = 2 * M_PI / (numPoints - 1);

    for (int j = 0; j < numPoints; j++)
    {
        float r = rand() % (80 - 20 + 1) + 20;
        float dx = r * cos( theta );
        float dy = r * sin( theta );

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;

        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const  
{
    if (switch_c % 2 == 0)
    {
        VertexArray lines(TriangleFan, m_numPoints + 1);

        Vector2f center = (Vector2f)target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane); ///POSSIBLE FIX
        lines[0].position = center;

        if (switch_c <= 2)
        {
            lines[0].color = m_color1;
            switch_c++;
        }
        else
        {
            lines[0].color = m_color2;
        }

        for (int j = 1; j <= m_numPoints; j++)
        {
            Vector2f coords; ///POSSIBLE FIX
            coords.x = m_A(0, j - 1);
            coords.y = m_A(1, j - 1);

            Vector2f pos = Vector2f{ target.mapCoordsToPixel(coords, m_cartesianPlane) };
            lines[j].position = pos;

            if (switch_c <= 2)
            {
                lines[j].color = Color::Blue;
                switch_c++;
            }
            else
            {
                lines[j].color = Color::Green;
                switch_c = 0;
            }
        }
        //cout << "OK OK OK" << endl;

        target.draw(lines); ///VIRTUAL DRAW
    }
    else
    {
        VertexArray lines(TrianglesFan, m_numPoints + 1);

        Vector2f center = (Vector2f)target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane); ///POSSIBLE FIX
        lines[0].position = center;

        if (switch_c <= 2)
        {
            lines[0].color = m_color1;
            switch_c++;
        }
        else
        {
            lines[0].color = m_color2;
        }

        for (int j = 1; j <= m_numPoints; j++)
        {
            Vector2f coords; ///POSSIBLE FIX
            coords.x = m_A(0, j - 1);
            coords.y = m_A(1, j - 1);

            Vector2f pos = Vector2f{ target.mapCoordsToPixel(coords, m_cartesianPlane) };
            lines[j].position = pos;

            if (switch_c <= 2)
            {
                lines[j].color = Color::Blue;
                switch_c++;
            }
            else
            {
                lines[j].color = Color::Green;
                switch_c = 0;
            }
        }
        //cout << "OK OK OK" << endl;

        target.draw(lines);
    }
}

void Particle::update(float dt) 
{
    m_ttl -= dt;
    rotate( dt * m_radiansPerSec );
    scale( SCALE );

    float dx = m_vx * dt;
    m_vy -= (G * dt);
    float dy = m_vy * dt;

    translate( dx, dy );
}


void Particle::rotate(double theta)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    RotationMatrix R = theta;
    m_A = R * m_A; ///MAKE SURE TO CHECK FOR LEFT_MULTIPLY R
    translate(temp.x, temp.y);
}

void Particle::scale(double c) 
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    ScalingMatrix S = c;
    m_A = S * m_A;
    translate(temp.x, temp.y);
}

void Particle::translate(double xShift, double yShift) 
{
    TranslationMatrix T(xShift, yShift, m_numPoints);
    m_A = T + m_A;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}