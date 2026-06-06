#ifndef PRESENTER_H
#define PRESENTER_H


#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <functional>
#include <future>
#include <sstream>
#include <cstdio>
#include <format>

#include "portable-file-dialogs.h"
#include "model.h"
#include "potentials.h"
#include "integrator.h"
#include "parser.h"


#define PI 3.14159


class Button {
protected:
    bool visible;
    bool pressed;
    std::string text;
    sf::Vector2f v1, v2;
    sf::Color col, text_col;
    std::function<void(void)> callback;
public:
    Button(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, sf::Color text_col_, std::string text_, bool visible_, std::function<void(void)> callback_);
    inline void set_visibility(bool visible_) { visible = visible_; }
    void process_click(sf::Vector2u click_pos, sf::Mouse::Button button);
    void process_release(sf::Vector2u release_pos, sf::Mouse::Button button);
    virtual void process_move(sf::Vector2u new_pos);
    virtual void draw(sf::RenderWindow &window);
    virtual ~Button() {}
};


class Textbox: public Button {
    bool entering;
public:
    Textbox(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, sf::Color text_col_, bool visible_);
    inline bool available() { return !entering && (text.size() > 0); }
    inline std::string get_text() { return text; }
    void process_enter(sf::Uint32 unicode);
    virtual ~Textbox() {}
};


class Slider: public Button {
    double pos;
public:
    Slider(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, double pos_, bool visible_);
    inline double get_pos() { return pos; }
    virtual void process_move(sf::Vector2u new_pos) override;
    virtual void draw(sf::RenderWindow &window) override;
    virtual ~Slider() {}
};


class Graph {
    bool visible;
    sf::Vector2f v1, v2;
    sf::Color col;
    std::vector<double> content;
public:
    Graph(sf::Vector2f v1_, sf::Vector2f v2_, sf::Color col_, bool visible_): v1(v1_), v2(v2_), col(col_), visible(visible_) {}
    inline void set_content(std::vector<double> content_) { content = content_; }
    void set_content(std::vector<vec3d> content_, int idx);
    inline void set_visibility(bool visible_) { visible = visible_; }
    virtual void draw(sf::RenderWindow &window);
    ~Graph() {}
};




class Presenter {
    int width, height;
    Model *M;

    std::string filename;
    Integrator_type integrator_type;
    Potential_type potential_type;
    int n_threads;
    double time_lim;
    double time_step;
    double save_step;

    double view_azim, view_elev;
    double dist_units_per_pixel;

    std::packaged_task<void(void)> task;
    std::future<void> result_future;
    std::thread model_thread;

    std::vector<std::vector<vec3d>> states;
    std::vector<vec3d> angular_momentum;
    std::vector<double> energy;
    std::vector<double> calc_time;
    int show_state_idx;
    bool got_results;

    void draw_background(sf::RenderWindow &window);
    void select_file();
    void start_calculation();
    void start_plotting();
    void draw_bodies(sf::RenderWindow &window);
    void show_error(Error *err);

public:
    Presenter(Model *M_): M(M_), filename(), n_threads(1), time_lim(1e5), time_step(0.1), save_step(100),
                         view_azim(0), view_elev(PI/2), dist_units_per_pixel(0.001), show_state_idx(0),
                         states(), angular_momentum(), energy(), calc_time(), integrator_type(Integrator_type::euler),
                         got_results(false) {
        width = sf::VideoMode::getDesktopMode().width;
        height = sf::VideoMode::getDesktopMode().height;
    }
    
    void start();
};


#endif
