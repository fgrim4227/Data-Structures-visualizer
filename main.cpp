#include <stdio.h>
#include<iostream>
#include <SFML/Graphics.hpp>
#include <VisualNode.hpp>
#include<BST.hpp>
template<typename T>
void drawTree(sf::RenderWindow& window, std::shared_ptr<BST<T>> root, sf::Vector2f pos, float hOffset, sf::Font& font) {
    if (!root) return;
    float vOffset = 80.f; // Distancia vertical entre niveles
    // 1. Dibujar líneas hacia los hijos primero
    if (root->getLeft()) {
        sf::Vertex line[] = {
            sf::Vertex(pos, sf::Color::Black),
            sf::Vertex(pos + sf::Vector2f(-hOffset, vOffset), sf::Color::Black)
        };
        window.draw(line, 4, sf::PrimitiveType::Lines);
        drawTree(window, root->getLeft(), pos + sf::Vector2f(-hOffset, vOffset), hOffset / 2.f, font);
    }

    if (root->getRight()) {
        sf::Vertex line[] = {
            sf::Vertex(pos, sf::Color::Black),
            sf::Vertex(pos + sf::Vector2f(hOffset, vOffset), sf::Color::Black)
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        drawTree(window, root->getRight(), pos + sf::Vector2f(hOffset, vOffset), hOffset / 2.f, font);
    }

    // 2. Dibujar el nodo (encima de las líneas)
    VisualNode nodeVisual(std::to_string(root->getData()), font);
    nodeVisual.setPosition(pos);
    window.draw(nodeVisual);
}
int main()
{
    /*
    Also could have been:
        sf::Window window;
        window.create(sf::VideoMode({800,600}), "My window");
    */
   
    sf::RenderWindow main_window(sf::VideoMode({800,600}), "My window", sf::Style::Default);
    //This line sets the position
    main_window.setPosition({0, 0});

    //This line sets the size
    main_window.setSize({900, 600});

    //This line sets the title
    main_window.setTitle("Testing");

    //This line gets the size of the window in an obect 'sf::Vector2u'
    sf::Vector2u size = main_window.getSize();

    //This line automaticallly creates a variable for each atribute of the size
    auto [width, height] = size;

    //This line checks if the window has the focus
    bool focus = main_window.hasFocus();

    //This line aligns the window's refresh rate with the monitor's
    //main_window.setVerticalSyncEnabled(true);

    //This line sets the framerate (not 100% reliable)
    main_window.setFramerateLimit(60);
    
    sf::Font font("assets/arial.ttf");
    auto cmp = [](int a, int b) { return a > b; };

    // Crear raíz y algunos nodos
    //auto tree = std::make_shared<BST<int>>(50, cmp);
    //tree->insert(30);
    //tree->insert(70);
    //tree->insert(20);
    //tree->insert(40);
    //tree->insert(60);
    //tree->insert(80);
    //tree->insert(10);
    std::shared_ptr<BST<int>> tree = nullptr;
    VisualNode test("Prueba",font);
    test.setPosition(sf::Vector2f(100, 200));
    //Don't ever mix setFrameRate and setVerticalSync.. at the same time
    //Run the program while the window is open
    int value_to_process = 0;
    while(main_window.isOpen())
    {
        //This loop checks the window's events that were triggered since the last iteration of the loop
        while(const std::optional event = main_window.pollEvent())
        {
            //If one of those events was a closing request then the window is closed
            if(event->is<sf::Event::Closed>())
            {
                main_window.close();
            }
            if(auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if(KeyPressed->code == sf::Keyboard::Key::A)
                {
                    std::cout<<"Ingrese el valor a insertar"<<std::endl;
                    std::cin >>value_to_process;
                    if(tree)
                    {
                        tree->insert(value_to_process);
                    }
                    else {
                        tree = std::make_shared<BST<int>>(value_to_process, cmp);
                    }
                }
                else if(KeyPressed->code == sf::Keyboard::Key::D)
                {
                    std::cout<<"Ingrese el valor a borrar"<<std::endl;
                    std::cin >>value_to_process;
                    if(tree)
                    {
                        tree = tree->delete_node(value_to_process);
                    }
                    else {
                        std::cout<<"No sea bruto"<<std::endl;
                    }
                }
            }
        }
        //This clears the window with the chosen color
        main_window.clear(sf::Color::White);
        
        //This is where you pass whatever you want to draw on the window
        //main_window.draw(...)
        //main_window.draw(test);
        if(tree) drawTree(main_window, tree, {400.f, 50.f}, 150.f, font);
        //SFML Offers 4 drawable entities
        //Sprites
        //Shapes
        //Texts
        //*Vertex arrays this one is special because it will allow you to create your own drawable entities
        //Ends the current frame, reveals whatever has been drawn since the last display
        main_window.display();
    }
}