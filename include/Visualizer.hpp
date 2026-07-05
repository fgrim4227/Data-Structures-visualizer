#include<SFML/Graphics.hpp>
#include<VisualNode.hpp>
#include<BST.hpp>
#include<AVL.hpp>
#include<Treap.hpp>
#include<Double_linked_list.hpp>
#include<imgui.h>
#include<imgui-SFML.h>
#include<functional>
#include<memory>
#include<cmath>
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
        AVLTree<T> avl_tree;        Treap<T> treap_tree;
        std::shared_ptr<D_Linked_list<T>> double_linked_list;
        std::function<bool(T,T)> cmp;
        void render_menu(sf::RenderWindow& window, sf::Clock delta_clock, sf::Font font);
        void draw_tree(sf::RenderWindow& window,std::shared_ptr<BST<T>> root, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font);
        void render_BST_visualizer(sf::RenderWindow& window, sf::Font& font);
        void draw_double_linked_list(sf::RenderWindow& window, sf::Font& font);
        void render_double_linked_list_visualizer(sf::RenderWindow& window, sf::Font& font);
        void draw_avl(sf::RenderWindow& window, AVLNode<T, std::less<T>>* node, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font);
        void render_AVL_visualizer(sf::RenderWindow& window, sf::Font& font);
        void draw_treap(sf::RenderWindow& window, TreapNode<T, std::less<T>>* node, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font);
        void render_Treap_visualizer(sf::RenderWindow& window, sf::Font& font);
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

            if(ImGui::MenuItem("Double Linked List")) current_state = State::Double_linked_list_visualizer;

            if(ImGui::MenuItem("AVL Visualizer")) current_state = State::AVL_visualizer;

            if(ImGui::MenuItem("Treap Visualizer")) current_state = State::Treap_visualizer;

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
            render_BST_visualizer(window, font);
            break;
        case State::Double_linked_list_visualizer:
            render_double_linked_list_visualizer(window, font);
            break;
        case State::AVL_visualizer:
            render_AVL_visualizer(window, font);
            break;
        case State::Treap_visualizer:
            render_Treap_visualizer(window, font);
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
/*
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
*/
template <typename T>
void Visualizer<T>::render_BST_visualizer(sf::RenderWindow& window,sf::Font& font)
{
    // --- 1. PANEL LATERAL FIJO ---
    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({220.f, static_cast<float>(window.getSize().y)}, ImGuiCond_Always);
    
    ImGui::Begin("Controles BST", nullptr, 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("<- Volver al Menú", {-1, 0})) {
        current_state = State::Main_menu;
    }
    
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Árbol Binario");
    ImGui::InputInt("Valor", &valueInput);        
    
    if (ImGui::Button("Insertar", {-1, 0})) {
        if (tree) {
            tree->insert(static_cast<T>(valueInput));
        } else {
            tree = std::make_shared<BST<T>>(static_cast<T>(valueInput), cmp);
        }
    }
    
    if (ImGui::Button("Eliminar", {-1, 0})) {
        if (tree) tree = tree->delete_node(static_cast<T>(valueInput));
    }

    ImGui::End();

    // --- 2. RENDERIZADO DEL ÁRBOL ---
    window.setView(view);
    
    if (tree) {
        // Le sumamos 100.f a X para que la raíz nazca más a la derecha y no quede detrás del menú
        sf::Vector2f start_pos = { (window.getSize().x / 2.f) + 100.f, 60.f };
        draw_tree(window, tree, start_pos, 150.f, 80.f, font);
    }
    
    window.setView(window.getDefaultView());
}
template <typename T>
void Visualizer<T>::draw_avl(sf::RenderWindow& window, AVLNode<T, std::less<T>>* node, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font) {
    if (!node) return;

    if (node->left) {
        sf::Vertex line[] = { sf::Vertex(pos, sf::Color::Black), sf::Vertex(pos + sf::Vector2f(-hOffset, vOffset), sf::Color::White) };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        draw_avl(window, node->left, pos + sf::Vector2f(-hOffset, vOffset), hOffset / 2.2f, vOffset, font);
    }
    if (node->right) {
        sf::Vertex line[] = { sf::Vertex(pos, sf::Color::Black), sf::Vertex(pos + sf::Vector2f(hOffset, vOffset), sf::Color::White) };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        draw_avl(window, node->right, pos + sf::Vector2f(hOffset, vOffset), hOffset / 2.2f, vOffset, font);
    }

    VisualNode nodeVisual(std::to_string(node->key), font, 35.f, sf::Color(120, 40, 200));
    nodeVisual.setPosition(pos);
    window.draw(nodeVisual);
}

template <typename T>
void Visualizer<T>::draw_treap(sf::RenderWindow& window, TreapNode<T, std::less<T>>* node, sf::Vector2f pos, float hOffset, float vOffset, sf::Font& font) {
    if (!node) return;

    if (node->left) {
        sf::Vertex line[] = { sf::Vertex(pos, sf::Color::Black), sf::Vertex(pos + sf::Vector2f(-hOffset, vOffset), sf::Color::White) };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        draw_treap(window, node->left, pos + sf::Vector2f(-hOffset, vOffset), hOffset / 2.2f, vOffset, font);
    }
    if (node->right) {
        sf::Vertex line[] = { sf::Vertex(pos, sf::Color::Black), sf::Vertex(pos + sf::Vector2f(hOffset, vOffset), sf::Color::White) };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        draw_treap(window, node->right, pos + sf::Vector2f(hOffset, vOffset), hOffset / 2.2f, vOffset, font);
    }

    // Treap: Mostramos el Key y un fragmento corto de la prioridad para que quepa visualmente
    std::string label = std::to_string(node->key) + "| P: " + std::to_string(node->priority % 1000);
    VisualNode nodeVisual(label, font, 45.f, sf::Color(200, 80, 20));
    nodeVisual.setPosition(pos);
    window.draw(nodeVisual);
}
template <typename T>
void Visualizer<T>::render_AVL_visualizer(sf::RenderWindow& window, sf::Font& font) {
    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({220.f, static_cast<float>(window.getSize().y)}, ImGuiCond_Always);
    ImGui::Begin("Controles AVL", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("<- Volver al Menú", {-1, 0})) current_state = State::Main_menu;
    
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Árbol AVL (Auto-balanceado)");
    ImGui::InputInt("Valor", &valueInput);        
    
    if (ImGui::Button("Insertar", {-1, 0})) {
        //if (!avl_tree.get_root()) avl_tree = AVLTree<T>();
        avl_tree.insert(static_cast<T>(valueInput));
    }
    if (ImGui::Button("Eliminar", {-1, 0})) {
        if (avl_tree.get_root()) avl_tree.erase(static_cast<T>(valueInput));
    }
    if (ImGui::Button("Limpiar AVL", {-1, 0})) {
        avl_tree.clean_tree();
    }
    ImGui::End();

    window.setView(view);
    if (avl_tree.get_root()) draw_avl(window, avl_tree.get_root(), { (window.getSize().x / 2.f) + 100.f, 60.f }, 150.f, 80.f, font);
    window.setView(window.getDefaultView());
}

template <typename T>
void Visualizer<T>::render_Treap_visualizer(sf::RenderWindow& window, sf::Font& font) {
    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({220.f, static_cast<float>(window.getSize().y)}, ImGuiCond_Always);
    ImGui::Begin("Controles Treap", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("<- Volver al Menú", {-1, 0})) current_state = State::Main_menu;
    
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Treap (Key + Priority)");
    ImGui::InputInt("Valor", &valueInput);        
    
    if (ImGui::Button("Insertar", {-1, 0})) {
        //if (!treap_tree.get_root()) treap_tree = Treap<T>();
        treap_tree.insert(static_cast<T>(valueInput));
    }
    
    ImGui::Separator();
    ImGui::Text("Opciones de Borrado:");
    if (ImGui::Button("Eliminar (Tradicional)", {-1, 0})) {
        if (treap_tree.get_root()) treap_tree.delete_node(static_cast<T>(valueInput));
    }
    if (ImGui::Button("Eliminar (Merge/Split)", {-1, 0})) {
        if (treap_tree.get_root()) treap_tree.delete_with_merge(static_cast<T>(valueInput));
    }
    
    if (ImGui::Button("Limpiar Treap", {-1, 0})) {
        treap_tree.clean_tree();
    }
    ImGui::End();

    window.setView(view);
    if (treap_tree.get_root()) draw_treap(window, treap_tree.get_root(), { (window.getSize().x / 2.f) + 100.f, 60.f }, 350.f, 200.f, font);
    window.setView(window.getDefaultView());
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
template<typename T>
void Visualizer<T>::draw_double_linked_list(sf::RenderWindow& window, sf::Font& font)
{
    if (!double_linked_list) return;

    auto centinel = double_linked_list->get_centinel();
    auto current = centinel->getNext();

    // 1. Contar cuántos nodos reales hay (sin contar el centinela)
    int node_count = 0;
    while (current != centinel && current != nullptr) {
        node_count++;
        current = current->getNext();
    }

    // Si solo está el centinela o está vacía, podemos dibujar solo el centinela en el centro
    if (node_count == 0) {
        VisualNode visual_centinel("C", font);
        visual_centinel.setPosition({ 400.f, 300.f });
        window.draw(visual_centinel);
        return;
    }

    // 2. Parámetros del círculo de la interfaz
    sf::Vector2f center = { 400.f, 300.f };
    float circle_radius = 180.f; // Radio del anillo general donde orbitan los nodos
    const float PI = 3.14159265f;

    // Guardaremos las posiciones para poder dibujar los enlaces (flechas/líneas) después
    std::vector<sf::Vector2f> positions;
    positions.reserve(node_count);

    // Recorremos de nuevo para calcular posiciones y dibujar nodos
    current = centinel->getNext();
    int i = 0;
    while (current != centinel && current != nullptr) {
        // Calcular ángulo en radianes
        float angle = i * (2.0f * PI / node_count);

        // Coordenadas cartesianas a partir de polares
        float x = center.x + circle_radius * std::cos(angle);
        float y = center.y + circle_radius * std::sin(angle);
        sf::Vector2f node_pos = { x, y };
        positions.push_back(node_pos);

        // Dibujar el nodo físico
        VisualNode node_visual(std::to_string(current->getData()), font);
        node_visual.setPosition(node_pos);
        window.draw(node_visual);

        current = current->getNext();
        i++;
    }

    // 3. Dibujar los enlaces dobles circulares
    // Para que se vea doble enlace, podemos hacer líneas sutilmente separadas o usar colores
    for (size_t idx = 0; idx < positions.size(); idx++) {
        sf::Vector2f from = positions[idx];
        sf::Vector2f to = positions[(idx + 1) % positions.size()]; // El siguiente (circular)

        // Enlace Next (Línea blanca o verde)
        sf::Vertex next_line[] = {
            sf::Vertex(from, sf::Color::Green),
            sf::Vertex(to, sf::Color::Green)
        };
        window.draw(next_line, 2, sf::PrimitiveType::Lines);

        // Enlace Prev (Sutilmente desplazado hacia el centro para que no se superpongan)
        sf::Vector2f offset_from = from + (center - from) * 0.1f;
        sf::Vector2f offset_to = to + (center - to) * 0.1f;

        sf::Vertex prev_line[] = {
            sf::Vertex(offset_from, sf::Color::Red),
            sf::Vertex(offset_to, sf::Color::Red)
        };
        window.draw(prev_line, 2, sf::PrimitiveType::Lines);
    }
}
template<typename T>
void Visualizer<T>::render_double_linked_list_visualizer(sf::RenderWindow& window, sf::Font& font)
{
ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({220.f, static_cast<float>(window.getSize().y)}, ImGuiCond_Always);
    
    ImGui::Begin("Controles Lista Doble", nullptr, 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("<- Volver al Menú", {-1, 0})) {
        current_state = State::Main_menu;
    }
    
    ImGui::Separator();
    ImGui::Spacing();

    static int list_value = 0;
    ImGui::InputInt("Valor", &list_value);

    if (ImGui::Button("Push Back", {-1, 0})) {
        if (!double_linked_list) {
            double_linked_list = std::make_shared<D_Linked_list<T>>();
        }
        double_linked_list->push_back(static_cast<T>(list_value));
    }

    if (ImGui::Button("Push Front", {-1, 0})) {
        if (!double_linked_list) double_linked_list = std::make_shared<D_Linked_list<T>>();
        double_linked_list->push_front(static_cast<T>(list_value));
    }

    if (ImGui::Button("Eliminar Nodo", {-1, 0})) {
        if (double_linked_list) double_linked_list->delete_node(static_cast<T>(list_value));
    }

    ImGui::End();

    // Renderizar la lista circular desplazando el mundo un poco a la derecha 
    // para que el menú de la izquierda no tape el círculo
    view.setCenter({ 350.f, 300.f }); 
    window.setView(view);

    draw_double_linked_list(window, font);

    window.setView(window.getDefaultView());
}