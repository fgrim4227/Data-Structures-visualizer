#include<SFML/Graphics.hpp>
#include<VisualNode.hpp>
#include<BST.hpp>
#include<imgui.h>
#include<imgui-SFML.h>
#include<functional>
enum class State
{
    Main_menu,
    BST_visualizer,
    Double_linked_list_visualizer,
    Single_linked_list_visualizer,
    Array_visualizer,
    Stack_visualizer,
    Queue_visualizer,
    BSTA_visualizer,
    Treap_visualizer,
    AVL_visualizer,
    END_PROGRAM
};
template <typename T>
class Visualizer
{
    //
    public:
        Visualizer(std::function<bool(T,T)> CMP) : current_state(State::Main_menu), tree(nullptr), cmp(CMP), valueInput(0), zoom_level(1.0f)
        {
            view.setSize({800.f, 600.f});
            view.setCenter({400.f, 300.f});
        }
        ~Visualizer() = default;
        void handle_events(sf::RenderWindow& window, const sf::Event& event);
        void update_and_run(sf::RenderWindow& window, sf::Clock delta_clock, sf::Font font);
    private:
        sf::View view;
        float zoom_level = 1.0f;
        bool is_dragging = false;
        sf::Vector2f last_mouse_pos;
        int valueInput;
        State current_state;
        std::shared_ptr<BST<T>> tree;
        std::function<bool(T,T)> cmp;
        void render_menu(sf::RenderWindow& window, sf::Clock delta_clock, sf::Font font);
        void draw_tree(sf::RenderWindow& window,std::shared_ptr<BST<T>> root, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font);
        void render_BST_visualizer(sf::RenderWindow& window,std::shared_ptr<BST<T>> root, sf::Font& font);
};
template<typename T>
void Visualizer<T>::handle_events(sf::RenderWindow& window, const sf::Event& event)
{
    if (ImGui::GetIO().WantCaptureMouse) return;

    if (const auto* mouse_wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouse_wheel->wheel == sf::Mouse::Wheel::Vertical) {
            float zoom_factor = (mouse_wheel->delta > 0) ? 0.9f : 1.1f;
            view.zoom(zoom_factor);
            zoom_level *= zoom_factor;
        }
    }

    if (const auto* mouse_button = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse_button->button == sf::Mouse::Button::Right) {
            is_dragging = true;
            // Guardamos la posición actual del mouse en coordenadas del mundo
            last_mouse_pos = window.mapPixelToCoords(mouse_button->position);
        }
    }

    if (const auto* mouse_button = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse_button->button == sf::Mouse::Button::Right) {
            is_dragging = false;
        }
    }

    if (const auto* mouse_move = event.getIf<sf::Event::MouseMoved>()) {
        if (is_dragging) {
            sf::Vector2f current_mouse_pos = window.mapPixelToCoords(mouse_move->position);
            sf::Vector2f delta = last_mouse_pos - current_mouse_pos;
            view.move(delta);
            // Actualizamos la posición para el siguiente frame de movimiento
            last_mouse_pos = window.mapPixelToCoords(mouse_move->position);
        }
    }
}
template <typename T>
void Visualizer<T>::update_and_run(sf::RenderWindow& window, sf::Clock delta_clock, sf::Font font)
{
    ImGui::SFML::Update(window, delta_clock.restart());
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Structures"))
        {
            if(ImGui::MenuItem("Binary Search Tree")) current_state = State::BST_visualizer;

            if(ImGui::MenuItem("Double Linked List (Coming Soon)")) current_state = State::Double_linked_list_visualizer;

            ImGui::Separator();

            if(ImGui::MenuItem("Exit")) window.close();

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Opciones")) {
            if (ImGui::MenuItem("Limpiar Pantalla")) tree = nullptr;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }


    switch(current_state){
        case State::Main_menu:
            render_menu(window, delta_clock, font);
            break;
        case State::BST_visualizer:
            render_BST_visualizer(window, tree, font);
            break;
        default:
            current_state = State::Main_menu;
    }

    ImGui::SFML::Render(window);

}
template <typename T>
void Visualizer<T>::draw_tree(sf::RenderWindow& window,std::shared_ptr<BST<T>> root, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font)
{    
    if (!root) return;
    // Distancia vertical entre niveles
    // 1. Dibujar líneas hacia los hijos primero
    if (root->getLeft()) {
        sf::Vertex line[] = {
            sf::Vertex(pos, sf::Color::Black),
            sf::Vertex(pos + sf::Vector2f(-hOffset, 80.f), sf::Color::White)
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        draw_tree(window, root->getLeft(), pos + sf::Vector2f(-hOffset, 80.f), hOffset / 2.2f, 80.f, font);
    }

    if (root->getRight()) {
        sf::Vertex line[] = {
            sf::Vertex(pos, sf::Color::Black),
            sf::Vertex(pos + sf::Vector2f(hOffset, 80.f), sf::Color::White)
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        draw_tree(window, root->getRight(), pos + sf::Vector2f(hOffset, 80.f), hOffset / 2.2f, 80.f, font);
    }

    // 2. Dibujar el nodo (encima de las líneas)
    VisualNode nodeVisual(std::to_string(root->getData()), font);
    nodeVisual.setPosition(pos);
    window.draw(nodeVisual);
}
template <typename T>
void Visualizer<T>::render_BST_visualizer(sf::RenderWindow& window,std::shared_ptr<BST<T>> root, sf::Font& font)
{
    window.setView(view);
    if(tree)
    {
        draw_tree(window, root, {window.getSize().x / 2.f, 60.f}, 100, 100, font);
    }
    window.setView(window.getDefaultView());
    ImGui::Begin("Controles BST", nullptr, ImGuiWindowFlags_MenuBar);
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("Options"))
        {
            ImGui::Text("Arbol Binario de Busqueda");
            ImGui::InputInt("Valor", &valueInput);        
            if (ImGui::Button("Insertar")) 
            {
                if (tree) 
                {
                    tree->insert(static_cast<T>(valueInput));
                    //addLog("Insertado: " + std::to_string(valueInput));
                } else 
                {
                    tree = std::make_shared<BST<T>>(static_cast<T>(valueInput), cmp);
                }
            }
            if(ImGui::Button("Eliminar"))
            {
                if(tree) tree = tree->delete_node(static_cast<T>(valueInput));
            }
            if (ImGui::Button("Volver al Menú")) 
            {
                current_state = State::Main_menu;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}
template<typename T>
void Visualizer<T>::render_menu(sf::RenderWindow& window, sf::Clock delta_clock, sf::Font font) {
    // Centrar la ventana de ImGui
    /*
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
    ImGui::SetNextWindowPos({window.getSize().x / 2.0f, window.getSize().y / 2.0f}, ImGuiCond_Always, {0.5f, 0.5f});
    ImGui::Begin("Seleccionar Estructura", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowFontScale(1.7f);
    if (ImGui::Button("Árbol Binario (BST)", {200, 50})) {
        current_state = State::BST_visualizer;
        //logs.push_back("Cambiado a modo BST."); // Mensaje para tu log
    }
    if (ImGui::Button("Grafos (Próximamente)", {200, 50})) {
        // Por ahora no hace nada
    }
    ImGui::End();
    */
   ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
   ImGui::SetNextWindowPos({window.getSize().x / 2.0f, window.getSize().y / 2.0f}, ImGuiCond_Always, {0.5f, 0.5f});
   if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("BST Visualizer")) {
        if (ImGui::MenuItem("New", "Ctrl+N")) 
        { 
            current_state = State::BST_visualizer; 
        }
        if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Handle Open */ }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit")) { /* Handle Exit */ }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

}