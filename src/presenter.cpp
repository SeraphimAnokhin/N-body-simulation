#include "presenter.h"


Button::Button(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, sf::Color text_col_, std::string text_,
               bool visible_, std::function<void(void)> callback_): v1(v1_), v2(v2_), pressed(false),
               col(col_), text_col(text_col_), text(text_), visible(visible_), callback(callback_) {}


void Button::process_click(sf::Vector2u click_pos, sf::Mouse::Button button) {
    if ((click_pos.x >= v1.x) && (click_pos.x <= v2.x) && (click_pos.y >= v1.y) && (click_pos.y <= v2.y)
        && (button == sf::Mouse::Left) && visible) {
        
        pressed = true;
    }
}

void Button::process_release(sf::Vector2u release_pos, sf::Mouse::Button button) {
    if ((release_pos.x >= v1.x) && (release_pos.x <= v2.x) && (release_pos.y >= v1.y) && (release_pos.y <= v2.y)
        && (button == sf::Mouse::Left) && pressed) {
        
        pressed = false;
        callback();
    }
}

void Button::process_move(sf::Vector2u new_pos) {
    if (((new_pos.x <= v1.x) || (new_pos.x >= v2.x) || (new_pos.y <= v1.y) || (new_pos.y >= v2.y)) && pressed) {
        pressed = false;
    }
}

void Button::draw(sf::RenderWindow &window) {
    if (!visible) {
        return;
    }

    sf::RectangleShape rechteckverteilung(sf::Vector2f(v2.x - v1.x, v2.y - v1.y));
    rechteckverteilung.setPosition(v1);
    rechteckverteilung.setFillColor(col);
    window.draw(rechteckverteilung);

    sf::Font font;
    if (!font.loadFromFile("../fonts/JetBrainsMono-SemiBold.ttf")) {
        throw Memory_error("Font file not found.");
    }

    sf::Text t;
    t.setFont(font);
    t.setString(text);
    t.setCharacterSize((v2.y - v1.y) * 0.7);  // pixels
    t.setFillColor(text_col);
    t.setPosition(v1);
    window.draw(t);
}


Textbox::Textbox(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, sf::Color text_col_, bool visible_):
            Button(v1_, v2_, col_, text_col_, "", visible_, [this](){ entering = true; }), entering(false) {}


void Textbox::process_enter(sf::Uint32 unicode) {
    if (!entering) {
        return;
    }

    if (unicode < 128) {
        if (unicode == 8) {                             // backspace
            if (text.size() > 0) {
                text.pop_back();
            }
        }
        else if ((unicode == 10) || (unicode == 13)) {  // CR or LF
            entering = false;
        }
        else {
            text.push_back(static_cast<char>(unicode));
        }
    }
}


Slider::Slider(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, double pos_, bool visible_):
                Button(v1_, v2_, col_, sf::Color::Transparent, "", visible_, [](){}), pos(pos_) {}


void Slider::process_move(sf::Vector2u new_pos) {
    if (pressed) {
        if (new_pos.x <= v1.x) {
            pos = 0;
        }
        else if (new_pos.x >= v2.x) {
            pos = 1;
        }
        else {
            pos = (new_pos.x - v1.x) / (v2.x - v1.x);
        }
    }
}


void Slider::draw(sf::RenderWindow &window) {
    Button::draw(window);

    if (!visible) {
        return;
    }

    sf::RectangleShape rechteckverteilung(sf::Vector2f(0.01 * (v2.x - v1.x), v2.y - v1.y));
    rechteckverteilung.setPosition(sf::Vector2f(v1.x + pos * (v2.x - v1.x), v1.y));
    rechteckverteilung.setFillColor(sf::Color::Black);
    window.draw(rechteckverteilung);
}


void Graph::set_content(std::vector<vec3d> content_, int idx) {
    content = std::vector<double>();
    for (int i = 0; i < content_.size(); i++) {
        switch (idx) {
        case 0:
            content.push_back(content_[i].x);
            break;
        case 1:
            content.push_back(content_[i].y);
            break;
        case 2:
            content.push_back(content_[i].z);
            break;
        default:
            break;
        }
    }
}

void Graph::draw(sf::RenderWindow &window) {
    if (!visible) {
        return;
    }

    sf::RectangleShape rechteckverteilung(sf::Vector2f(v2.x - v1.x, v2.y - v1.y));
    rechteckverteilung.setPosition(v1);
    rechteckverteilung.setFillColor(col);
    window.draw(rechteckverteilung);

    double max = 0, min = 0;
    if (content.size() > 0) {
        max = content[0];
        min = content[0];
    }

    for (int i = 1; i < content.size(); i++) {
        if (content[i] > max) {
            max = content[i];
        }
        if (content[i] < min) {
            min = content[i];
        }
    }

    sf::Font font;
    if (!font.loadFromFile("../fonts/JetBrainsMono-SemiBold.ttf")) {
        throw Memory_error("Font file not found.");
    }

    char s[100];
    std::sprintf(s, "%e", max);

    sf::Text t;
    t.setFont(font);
    t.setString(std::string("max: ") + std::string(s));
    t.setCharacterSize((v2.y - v1.y) * 0.07);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(v1.x - 0.2 * (v2.x - v1.x), v1.y - 0.05 * (v2.y - v1.y)));
    window.draw(t);

    std::sprintf(s, "%e", min);

    t.setFont(font);
    t.setString(std::string("min: ") + std::string(s));
    t.setCharacterSize((v2.y - v1.y) * 0.07);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(v1.x - 0.2 * (v2.x - v1.x), v2.y + 0.05 * (v2.y - v1.y)));
    window.draw(t);

    for (int i = 0; i < content.size(); i++) {
        sf::CircleShape point(0.005 * (v2.y - v1.y));
        point.setFillColor(sf::Color::Green);
        point.setPosition(sf::Vector2f(v1.x + 1.0 * (v2.x - v1.x) * i / content.size(), v1.y + (max - content[i]) / (max - min) * (v2.y - v1.y)));
        window.draw(point);
    }
}

void Presenter::draw_background(sf::RenderWindow &window) {
    window.clear(sf::Color::White);

    sf::Font font;
    if (!font.loadFromFile("../fonts/JetBrainsMono-SemiBold.ttf")) {
        throw Memory_error("Font file not found.");
    }

    sf::RectangleShape left_panel(sf::Vector2f(0.25 * width, height));
    left_panel.setPosition(sf::Vector2f(0, 0));
    left_panel.setFillColor(sf::Color(208, 154, 252));
    window.draw(left_panel);

    sf::RectangleShape right_panel(sf::Vector2f(0.25 * width, height));
    right_panel.setPosition(sf::Vector2f(0.75 * width, 0));
    right_panel.setFillColor(sf::Color(255, 128, 0));
    window.draw(right_panel);

    sf::RectangleShape file_selection_panel(sf::Vector2f(0.23 * width, 0.15 * height));
    file_selection_panel.setPosition(sf::Vector2f(0.76 * width, 0.01 * height));
    file_selection_panel.setFillColor(sf::Color(139, 235, 223));
    window.draw(file_selection_panel);

    if (filename.size() > 0) {
        sf::Text t;
        t.setFont(font);
        t.setString(std::string("File selected:\n") + filename.substr(filename.find_last_of(pfd::path::separator()) + 1));
        t.setCharacterSize(0.02 * height);  // pixels
        t.setFillColor(sf::Color::Black);
        t.setPosition(sf::Vector2f(0.78 * width, 0.1 * height));
        window.draw(t);
    }
    else {
        sf::Text t;
        t.setFont(font);
        t.setString(std::string("File not selected."));
        t.setCharacterSize(0.02 * height);  // pixels
        t.setFillColor(sf::Color::Black);
        t.setPosition(sf::Vector2f(0.78 * width, 0.1 * height));
        window.draw(t);
    }

    sf::RectangleShape integration_panel(sf::Vector2f(0.23 * width, 0.26 * height));
    integration_panel.setPosition(sf::Vector2f(0.76 * width, 0.18 * height));
    integration_panel.setFillColor(sf::Color(139, 235, 223));
    window.draw(integration_panel);

    sf::Text t;
    t.setFont(font);
    t.setString(std::string("Integrator"));
    t.setCharacterSize(0.038 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.78 * width, 0.18 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("N_thr"));
    t.setCharacterSize(0.02 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.78 * width, 0.26 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("time\nlim"));
    t.setCharacterSize(0.02 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.8 * width, 0.3 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("time\nstep"));
    t.setCharacterSize(0.02 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.9 * width, 0.23 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("save\nstep"));
    t.setCharacterSize(0.02 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.9 * width, 0.3 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("Integrator\ntype:"));
    t.setCharacterSize(0.018 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.76 * width, 0.37 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("azim: ") + std::to_string(int(view_azim * 180 / PI)));
    t.setCharacterSize(0.02 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.6 * width, 0.03 * height));
    window.draw(t);

    t.setFont(font);
    t.setString(std::string("elev: ") + std::to_string(int(view_elev * 180 / PI)));
    t.setCharacterSize(0.02 * height);  // pixels
    t.setFillColor(sf::Color::Black);
    t.setPosition(sf::Vector2f(0.6 * width, 0.07 * height));
    window.draw(t);

}

void Presenter::select_file() {
    // Open native file dialog
    auto selection = pfd::open_file("Select a file", ".",
        { "Text Files (.txt)", "*.txt",
          "All Files (*)", "*" }).result();

    if (!selection.empty()) {
        filename = selection[0];
        std::vector<double> m;
        std::vector<vec3d> r0, v0;
        Parser::parse(filename, m, r0, v0);
        M->set_initial_state(m, r0, v0);
        M->set_n_bodies(m.size());
    }
}

void Presenter::start_calculation() {
    got_results = false;

    M->set_integrator_type(integrator_type);
    M->set_time_lim(time_lim);
    M->set_time_step(time_step);
    M->set_state_save_step(save_step);
    M->set_n_threads(n_threads);

    task = std::packaged_task<void(void)>([=](){M->start_calculation();});
    result_future = task.get_future();
    model_thread = std::thread(std::move(task));
    model_thread.detach();
}

void Presenter::start_plotting() {
    states = M->get_states();
    angular_momentum = M->get_angular_momentum();
    energy = M->get_energy();
    calc_time = M->get_calc_time();
}

void Presenter::draw_bodies(sf::RenderWindow &window) {
    if (states.size() > 0) {
        for (int i = 0; i < states[show_state_idx].size(); i++) {

            vec3d view(1, view_azim, view_elev, true);
            vec3d r_draw = states[show_state_idx][i] - view * (states[show_state_idx][i] * view) / (view * view);
            r_draw = states[show_state_idx][i];
            r_draw /= dist_units_per_pixel;

            sf::CircleShape point(0.005 * height);
            point.setFillColor(sf::Color::Magenta);
            point.setPosition(sf::Vector2f(r_draw.x + 0.5 * width, r_draw.y + 0.5 * height));
            window.draw(point);
        }
    }
}

void Presenter::show_error(Error *err) {
    pfd::notify("Error",
        err->what(),
        pfd::icon::info);
}

void Presenter::start() {

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "N body simulation");

    M->set_potential_type(Potential_type::newton);    

    int n_buttons = 2;
    int n_textboxes = 5;
    int n_sliders = 1;
    Button *buttons[n_buttons + n_textboxes + n_sliders];

    buttons[0] = new Button(sf::Vector2f(0.8 * width, 0.02 * height), sf::Vector2f(0.95 * width, 0.07 * height), sf::Color::Red, sf::Color::Black, "Select file", true, [this](){ select_file(); });
    buttons[1] = new Button(sf::Vector2f(0.8 * width, 0.52 * height), sf::Vector2f(0.99 * width, 0.57 * height), sf::Color::Red, sf::Color::Black, "Start calculation", true, [this](){ start_calculation(); });

    buttons[n_buttons + 0] = new Textbox(sf::Vector2f(0.83 * width, 0.24 * height), sf::Vector2f(0.88 * width, 0.29 * height), sf::Color::White, sf::Color::Black, true);
    buttons[n_buttons + 1] = new Textbox(sf::Vector2f(0.83 * width, 0.3 * height), sf::Vector2f(0.88 * width, 0.35 * height), sf::Color::White, sf::Color::Black, true);
    buttons[n_buttons + 2] = new Textbox(sf::Vector2f(0.93 * width, 0.24 * height), sf::Vector2f(0.98 * width, 0.29 * height), sf::Color::White, sf::Color::Black, true);
    buttons[n_buttons + 3] = new Textbox(sf::Vector2f(0.93 * width, 0.3 * height), sf::Vector2f(0.98 * width, 0.35 * height), sf::Color::White, sf::Color::Black, true);
    buttons[n_buttons + 4] = new Textbox(sf::Vector2f(0.83 * width, 0.37 * height), sf::Vector2f(0.98 * width, 0.42 * height), sf::Color::White, sf::Color::Black, true);

    buttons[n_buttons + n_textboxes + 0] = new Slider(sf::Vector2f(0.3 * width, 0.83 * height), sf::Vector2f(0.7 * width, 0.86 * height), sf::Color::Cyan, 0.0, true);

    int n_graphs = 5;
    Graph *graphs[n_graphs];

    graphs[0] = new Graph(sf::Vector2f(0.05 * width, 0.02 * height), sf::Vector2f(0.2 * width, 0.2 * height), sf::Color::White, true);
    graphs[1] = new Graph(sf::Vector2f(0.05 * width, 0.25 * height), sf::Vector2f(0.2 * width, 0.43 * height), sf::Color::White, true);
    graphs[2] = new Graph(sf::Vector2f(0.05 * width, 0.47 * height), sf::Vector2f(0.2 * width, 0.65 * height), sf::Color::White, true);
    graphs[3] = new Graph(sf::Vector2f(0.05 * width, 0.69 * height), sf::Vector2f(0.2 * width, 0.87 * height), sf::Color::White, true);
    graphs[4] = new Graph(sf::Vector2f(0.8 * width, 0.69 * height), sf::Vector2f(0.95 * width, 0.87 * height), sf::Color::White, true);

    while (window.isOpen()) {
        try {
            try {
                draw_background(window);
            }
            catch(Incorrect_input &err) {
                show_error(&err);
            }
        }
        catch(Incorrect_input &err) {
            show_error(&err);
        }

        // reading from textboxes

        if (static_cast<Textbox *> (buttons[n_buttons + 0])->available()) {
            std::string s = static_cast<Textbox *> (buttons[n_buttons + 0])->get_text();
            std::istringstream in(s);
            in >> n_threads;
        }
        if (static_cast<Textbox *> (buttons[n_buttons + 1])->available()) {
            std::string s = static_cast<Textbox *> (buttons[n_buttons + 1])->get_text();
            std::istringstream in(s);
            in >> time_lim;
        }
        if (static_cast<Textbox *> (buttons[n_buttons + 2])->available()) {
            std::string s = static_cast<Textbox *> (buttons[n_buttons + 2])->get_text();
            std::istringstream in(s);
            in >> time_step;
        }
        if (static_cast<Textbox *> (buttons[n_buttons + 3])->available()) {
            std::string s = static_cast<Textbox *> (buttons[n_buttons + 3])->get_text();
            std::istringstream in(s);
            in >> save_step;
        }
        if (static_cast<Textbox *> (buttons[n_buttons + 4])->available()) {
            std::string s = static_cast<Textbox *> (buttons[n_buttons + 4])->get_text();

            if (s == std::string("euler")) {
                integrator_type = Integrator_type::euler;
            }
            else if (s == std::string("leapfrog")) {
                integrator_type = Integrator_type::leapfrog;
            }
            else if (s == std::string("rk4")) {
                integrator_type = Integrator_type::rk4;
            }
            else {
                // Incorrect_input err("Unknown integrator type!");
                // show_error(&err);
            }
        }

        // reading from slider

        double p = static_cast<Slider *> (buttons[n_buttons + n_textboxes + 0])->get_pos();
        show_state_idx = (states.size() - 1) * p;

        // progress bar

        sf::Font font;
        if (!font.loadFromFile("../fonts/JetBrainsMono-SemiBold.ttf")) {
            Memory_error err("Font file not found.");
            show_error(&err);
        }

        sf::Text t;
        t.setFont(font);
        t.setString(std::to_string(M->get_progress()));
        t.setCharacterSize(0.02 * height);  // pixels
        t.setFillColor(sf::Color::Black);
        t.setPosition(sf::Vector2f(0.78 * width, 0.6 * height));
        window.draw(t);

        // plotting if possible

        if (M->is_ready() && (M->get_progress() > 0.9) && !got_results) {
            try {
                //M->set_ready(false);
                got_results = true;
                result_future.get();
                start_plotting();
                graphs[0]->set_content(energy);
                graphs[1]->set_content(angular_momentum, 0);
                graphs[2]->set_content(angular_momentum, 1);
                graphs[3]->set_content(angular_momentum, 2);
                graphs[4]->set_content(calc_time);
                //std::cout << "done\n";
            } 
            catch (Incorrect_input &err) {
                show_error(&err);
                std::cout << err.what() << "\n";
            }
        }

        draw_bodies(window);
        for (int i = 0; i < n_graphs; i++) {
            try {
                graphs[i]->draw(window);
            }
            catch(Incorrect_input &err) {
                show_error(&err);
            }
        }


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                Parser::save("result.txt", states);
                M->stop_calculation();
                window.close();
            }
                

            if (event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window); // пиксели мыши
                sf::Vector2f pos = window.mapPixelToCoords(mousePos);   // world coords
                for (int i = 0; i < n_buttons + n_textboxes + n_sliders; i++) {
                    buttons[i]->process_click(sf::Vector2u(pos.x, pos.y), event.mouseButton.button);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window); // пиксели мыши
                sf::Vector2f pos = window.mapPixelToCoords(mousePos);   // world coords
                for (int i = 0; i < n_buttons + n_textboxes + n_sliders; i++) {
                    buttons[i]->process_release(sf::Vector2u(pos.x, pos.y), event.mouseButton.button);
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window); // пиксели мыши
                sf::Vector2f pos = window.mapPixelToCoords(mousePos);   // world coords
                for (int i = 0; i < n_buttons + n_textboxes + n_sliders; i++) {
                    buttons[i]->process_move(sf::Vector2u(pos.x, pos.y));
                }
            }
            if (event.type == sf::Event::TextEntered) {
                for (int i = 0; i < n_textboxes; i++) {
                    static_cast<Textbox *> (buttons[n_buttons + i])->process_enter(event.text.unicode);
                }
                if (event.key.code == 'a') {
                    view_azim += 0.05 * PI;
                }
                else if (event.key.code == 'd') {
                    view_azim -= 0.05 * PI;
                }
                else if (event.key.code == 'w') {
                    view_elev += 0.05 * PI;
                }
                else if (event.key.code == 's') {
                    view_elev -= 0.05 * PI;
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (event.mouseWheelScroll.delta > 0) {
                        dist_units_per_pixel *= 2;
                        if (dist_units_per_pixel == 0) {
                            dist_units_per_pixel = 1e-10;
                        }
                    }
                    else {
                        dist_units_per_pixel /= 2;
                        if (dist_units_per_pixel == 0) {
                            dist_units_per_pixel = 1e-10;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < n_buttons + n_textboxes + n_sliders; i++) {
            try {
                buttons[i]->draw(window);
            }
            catch(Incorrect_input &err) {
                show_error(&err);
            }
        }
        
        window.display();
    }
    for (int i = 0; i < n_buttons + n_textboxes + n_sliders; i++) {
        delete buttons[i];
    }
    for (int i = 0; i < n_graphs; i++) {
        delete graphs[i];
    }
}


