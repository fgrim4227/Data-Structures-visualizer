#include <stdio.h>
#include<iostream>
/*#include <SFML/Graphics.hpp>
#include <VisualNode.hpp>
#include<BST.hpp>
#include<imgui.h>
#include<imgui-SFML.h>*/
#include <Visualizer.hpp>

int main()
{
    sf::RenderWindow main_window(sf::VideoMode({800, 600}), "Data Structures Visualizer", sf::Style::Default);
    main_window.setFramerateLimit(60);
    main_window.setPosition({0, 0});

    if(!ImGui::SFML::Init(main_window)) return -1;

    sf::Font font("assets/arial.ttf");
    auto cmp = [](int a, int b) { return a > b; };
    Visualizer<int> Main(cmp);
    //std::shared_ptr<BST<int>> tree = nullptr;

    int value_input = 0;
    sf::Clock deltaClock;

    while(main_window.isOpen())
    {
        while(const std::optional event = main_window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(main_window, *event);
            Main.handle_events(main_window, *event);
            if(event->is<sf::Event::Closed>()) main_window.close();
        }
        /*
        ImGui::SFML::Update(main_window, deltaClock.restart());
        ImGui::Begin("Panel de Control");

        ImGui::Text("Arbol Binario de Busqueda");
        ImGui::InputInt("Valor", &value_input);

        if(ImGui::Button("Insertar"))
        {
            if(tree) tree->insert(value_input);
            else tree = std::make_shared<BST<int>>(value_input, cmp);
        }
        ImGui::SameLine();
        if(ImGui::Button("Borrar"))
        {
            if(tree) tree = tree->delete_node(value_input);
        }
        ImGui::End();

        main_window.clear(sf::Color(45,45,45));

        if(tree) drawTree(main_window, tree, {400.f, 50.f}, 150.f, font);

        ImGui::SFML::Render(main_window);*/
        main_window.clear(sf::Color(45,45,45));
        Main.update_and_run(main_window,deltaClock,font);
        main_window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}