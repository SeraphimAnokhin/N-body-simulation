#include <SFML/Graphics.hpp>
#include "model.h"
#include "presenter.h"
#include "parser.h"
#include <string>
#include "generator.h"

int main(int argc, char **argv) {

    int n_bodies;
    if (argc > 3) {
        n_bodies = atoi(argv[2]);
        if (argv[1][0] == 'r') {
            Random R;
            R.set_n(n_bodies);
            R.generate(argv[3]);
        }
        else if (argv[1][0] == 'c') {
            Cold_compression C;
            C.set_n(n_bodies);
            C.generate(argv[3]);
        }
        else if (argv[1][0] == '2') {
            Two_body T;
            T.generate(argv[3]);
        }
    }
    else {
        Model M;
        Presenter P(&M);
        P.start();
    }

    return 0;
}
