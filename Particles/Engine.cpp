#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>

int switch_ = 0; //Switch for BG Change


Engine::Engine()
{
    m_Window.create(VideoMode::getDesktopMode(), "Particles");
}

void Engine::run()
{
    Clock local_clock; //TRACK FPS

    ////------------------------------EXTRA CREDIT-----------------------------------------------------

    SoundBuffer warpBuffer;
    warpBuffer.loadFromFile("flume.wav");
    Sound warp;
    warp.setBuffer(warpBuffer);

    Texture textureBackground;

    //// Load a graphic into the texture
    //textureBackground.loadFromFile("CrystalShard.png");
    //Sprite spriteBackground;

    //// Attach the texture to the sprite
    //spriteBackground.setTexture(textureBackground);

    //// Set the spriteBackground to cover the screen
    //spriteBackground.setPosition(0, 0);
    warp.play();

    ////------------------------------------------------------------------------------------------------

    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;

    while (m_Window.isOpen())
    {
       Time time = local_clock.getElapsedTime(); // .asSeconds() for seconds (float)
       local_clock.restart();

       input();
       update(time.asSeconds());
       draw();
    }
}

void Engine::input()
{
    Event event;

    while (m_Window.pollEvent(event))
    {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            m_Window.close();
        }
        if (event.type == Event::Closed)
        {
            m_Window.close();
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            switch_++;
            if (switch_ >= 3) switch_ = 1;

            int num_points = rand() % (50 - 25 + 1) + 26;
            int points = 5;

            for (int i = 0; i < points; i++){
                Particle p(m_Window, num_points, Mouse::getPosition(m_Window)); //RECHECK ONCE PARTICLE_H IS FINISHED
                m_particles.push_back(p);
                //cout << "POINTS ADDED" << endl;
            }
        }
    }
}
    
void Engine::update(float dtAsSeconds) 
{
    ////TRAD FOR-LOOP////
    //for (int i = 0; i < m_particles.size(); i++) //Change to iterator based for-loop
    //{
    //    if (m_particles[i].getTTL() > 0.0)
    //    {
    //        float TTL = m_particles[i].getTTL();
    //        m_particles[i].Particle::update(TTL);
    //    }
    //    else
    //    {
    //        m_particles.erase(m_particles.begin() + i);
    //        --i;
    //    }
    //}

    ////ITERATOR BASED////////
    for (auto iter = m_particles.begin(); iter != m_particles.end();)
    {
        if (iter->getTTL() > 0.0)
        {
            iter->update(dtAsSeconds);
            ++iter;
        }
        else
        {
            iter = m_particles.erase(iter);
        }
    }
}

void Engine::draw() 
{
    //if (switch_ == 1)
    //{
    //    m_Window.clear(Color::White);
    //    switch_ = 2;
    //}
    //else if (switch_ == 2)
    //{
    //    m_Window.clear(Color::Black);
    //    switch_ = 3;
    //}
    //else if (switch_ == 3)
    //{
    //    m_Window.clear(Color::Green);
    //    switch_ = 1;
    //}
    // 
    // 
    m_Window.clear();
    ////FOR RANGE
    for (const auto& p : m_particles) //might need to change for USEABILITY
    {
       m_Window.draw(p);
    }

    ///TRAD FOR
    //for (int i = 0; i < m_particles.size(); i++)
    //{
    //    m_Window.draw(m_particles[i]);
    //}
    m_Window.display();
}

