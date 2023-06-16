#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHAPES 10
#define MAX_COMMAND_LEN 100

typedef enum { POINT, LINE, SQUARE, RECTANGLE, CIRCLE, POLYGON } SHAPE_TYPE;
typedef enum { ADD, DELETE, LIST, CLEAR, QUIT, HELP, SHAPE } COMMAND_TYPE;

typedef struct {
    int pos_x;
    int pos_y;
} Point;

typedef struct {
    Point *p1;
    Point *p2;
} Line;

typedef struct {
    Point *point;
    int length;
} Square;

typedef struct {
    Point *point;
    int width;
    int height;
} Rectangle;

typedef struct {
    Point *center;
    int radius;
} Circle;

typedef struct {
    int n;
    Point **points;
} Polygon;

typedef struct shape {
    int id; 
    SHAPE_TYPE shape_type; 
    void *ptrShape; 
} Shape;

typedef struct {
    COMMAND_TYPE type;
    SHAPE_TYPE shape_type;
    int params[8];
    int num_params;
} Command;

unsigned long long int global_id = 0;
Shape *shapes[MAX_SHAPES];
int shape_count = 0;

unsigned long long int get_next_id() {
    return ++global_id;
}

void set_id(unsigned long long int id) {
    global_id = id;
}

Point *create_point(int px, int py) {
    Point *p = malloc(sizeof(Point));
    p->pos_x = px;
    p->pos_y = py;
    return p;
}

void delete_point(Point *p) {
    free(p);
}

void print_point(Point *p) {
    printf("POINT [%d, %d]\n\n", p->pos_x, p->pos_y);
}

Line *create_line(Point *p1, Point *p2) {
    Line *l = malloc(sizeof(Line));
    l->p1 = p1;
    l->p2 = p2;
    return l;
}

void delete_line(Line *l) {
    free(l);
}

void print_line(Line *l) {
    printf("LINE [%d, %d], [%d, %d]\n\n", l->p1->pos_x, l->p1->pos_y, l->p2->pos_x, l->p2->pos_y);
}

Square *create_square(Point *point, int length) {
    Square *s = malloc(sizeof(Square));
    s->point = point;
    s->length = length;
    return s;
}

void delete_square(Square *s) {
    free(s);
}

void print_square(Square *s) {
    printf("SQUARE [%d, %d], length %d\n\n", s->point->pos_x, s->point->pos_y, s->length);
}

Rectangle *create_rectangle(Point *point, int width, int height) {
    Rectangle *r = malloc(sizeof(Rectangle));
    r->point = point;
    r->width = width;
    r->height = height;
    return r;
}

void delete_rectangle(Rectangle *r) {
    free(r);
}

void print_rectangle(Rectangle *r) {
    printf("RECTANGLE [%d, %d], width %d, height %d\n\n", r->point->pos_x, r->point->pos_y, r->width, r->height);
}

Circle *create_circle(Point *center, int radius) {
    Circle *c = malloc(sizeof(Circle));
    c->center = center;
    c->radius = radius;
    return c;
}

void delete_circle(Circle *c) {
    free(c);
}

void print_circle(Circle *c) {
    printf("CIRCLE [%d, %d], radius %d\n\n", c->center->pos_x, c->center->pos_y, c->radius);
}

Polygon *create_polygon(int n) {
    Polygon *pg = malloc(sizeof(Polygon));
    pg->n = n;
    pg->points = malloc(n * sizeof(Point *));
    return pg;
}

void delete_polygon(Polygon *pg) {
    for (int i = 0; i < pg->n; i++) {
        free(pg->points[i]);
    }
    free(pg->points);
    free(pg);
}

void print_polygon(Polygon *pg) {
    printf("POLYGON ");
    for (int i = 0; i < pg->n; i++) {
        printf("[%d, %d] ", pg->points[i]->pos_x, pg->points[i]->pos_y);
    }
    printf("\n\n");
}

Shape *create_empty_shape(SHAPE_TYPE shape_type) {
    Shape *shp = (Shape *) malloc(sizeof(Shape));
    shp->ptrShape = NULL;
    shp->id = get_next_id();
    shp->shape_type = shape_type;
    return shp;
}

Shape *create_point_shape(int px, int py) {
    Shape *shp = create_empty_shape(POINT);
    Point *p = create_point(px, py);
    shp->ptrShape = p;
    return shp;
}

Shape *create_line_shape(int px1, int py1, int px2, int py2) {
    Shape *shp = create_empty_shape(LINE);
    Point *p1 = create_point(px1, py1);
    Point *p2 = create_point(px2, py2);
    Line *l = create_line(p1, p2);
    shp->ptrShape = l;
    return shp;
}

Shape *create_square_shape(int px, int py, int length) {
    Shape *shp = create_empty_shape(SQUARE);
    Point *p = create_point(px, py);
    Square *s = create_square(p, length);
    shp->ptrShape = s;
    return shp;
}

Shape *create_rectangle_shape(int px, int py, int width, int height) {
    Shape *shp = create_empty_shape(RECTANGLE);
    Point *p = create_point(px, py);
    Rectangle *r = create_rectangle(p, width, height);
    shp->ptrShape = r;
    return shp;
}

Shape *create_circle_shape(int px, int py, int radius) {
    Shape *shp = create_empty_shape(CIRCLE);
    Point *p = create_point(px, py);
    Circle *c = create_circle(p, radius);
    shp->ptrShape = c;
    return shp;
}

Shape *create_polygon_shape(int lst[], int n) {
    Shape *shp = create_empty_shape(POLYGON);
    Polygon *pg = create_polygon(n);
    for (int i = 0; i < n; i+=2) {
        pg->points[i/2] = create_point(lst[i], lst[i+1]);
    }
    shp->ptrShape = pg;
    return shp;
}

void delete_shape(Shape *shape) {
    switch (shape->shape_type) {
        case POINT:
            delete_point((Point *)shape->ptrShape);
            break;
        case LINE:
            delete_line((Line *)shape->ptrShape);
            break;
        case SQUARE:
            delete_square((Square *)shape->ptrShape);
            break;
        case RECTANGLE:
            delete_rectangle((Rectangle *)shape->ptrShape);
            break;
        case CIRCLE:
            delete_circle((Circle *)shape->ptrShape);
            break;
        case POLYGON:
            delete_polygon((Polygon *)shape->ptrShape);
           
break;
    }
    free(shape);
}

void print_shape(Shape *shape) {
    printf("Shape ID: %d, Type: ", shape->id);
    switch (shape->shape_type) {
        case POINT:
            printf("POINT\n");
            print_point((Point *)shape->ptrShape);
            break;
        case LINE:
            printf("LINE\n");
            print_line((Line *)shape->ptrShape);
            break;
        case SQUARE:
            printf("SQUARE\n");
            print_square((Square *)shape->ptrShape);
            break;
        case RECTANGLE:
            printf("RECTANGLE\n");
            print_rectangle((Rectangle *)shape->ptrShape);
            break;
        case CIRCLE:
            printf("CIRCLE\n");
            print_circle((Circle *)shape->ptrShape);
            break;
        case POLYGON:
            printf("POLYGON\n");
            print_polygon((Polygon *)shape->ptrShape);
            break;
    }
}

void execute_command(Command cmd);

Command parse_command(char *input) {
    Command cmd;
    char command_str[MAX_COMMAND_LEN];
    char shape_str[MAX_COMMAND_LEN];
    char* token;
    int index = 0;

    sscanf(input, "%s %s", command_str, shape_str);
    
    // Conversion des chaînes de commande et de forme en types d'énumération.
    if(strcmp(command_str, "ADD") == 0) {
        cmd.type = ADD;
    } else if(strcmp(command_str, "DELETE") == 0) {
        cmd.type = DELETE;
    } else if(strcmp(command_str, "LIST") == 0) {
        cmd.type = LIST;
    } else if(strcmp(command_str, "CLEAR") == 0) {
        cmd.type = CLEAR;
    } else if(strcmp(command_str, "QUIT") == 0) {
        cmd.type = QUIT;
    } else if(strcmp(command_str, "HELP") == 0) {
        cmd.type = HELP;
    } else if(strcmp(command_str, "SHAPE") == 0) {
        cmd.type = SHAPE;
    } else
    {
        printf("command not found\n");
    }
    

    if(strcmp(shape_str, "POINT") == 0) {
        cmd.shape_type = POINT;
    } else if(strcmp(shape_str, "LINE") == 0) {
        cmd.shape_type = LINE;
    } else if(strcmp(shape_str, "SQUARE") == 0) {
        cmd.shape_type = SQUARE;
    } else if(strcmp(shape_str, "RECTANGLE") == 0) {
        cmd.shape_type = RECTANGLE;
    } else if(strcmp(shape_str, "CIRCLE") == 0) {
        cmd.shape_type = CIRCLE;
    } else if(strcmp(shape_str, "POLYGON") == 0) {
        cmd.shape_type = POLYGON;
    }
    
    // Récupération des paramètres.
    token = strtok(input, " "); // la première commande
    token = strtok(NULL, " "); // la deuxième commande ou la forme
    token = strtok(NULL, " "); // premier paramètre

    while(token != NULL) {
        cmd.params[index] = atoi(token);
        index++;
        token = strtok(NULL, " ");
    }

    cmd.num_params = index;

    return cmd;
}

void execute_command(Command cmd) {
    Shape *shape;
    switch (cmd.type) {
        case ADD:
            // Ajoutez une nouvelle forme à la liste de formes.
            switch (cmd.shape_type) {
                case POINT:
                    shape = create_point_shape(cmd.params[0], cmd.params[1]);
                    break;
                case LINE:
                    shape = create_line_shape(cmd.params[0], cmd.params[1], cmd.params[2], cmd.params[3]);
                    break;
                case SQUARE:
                    shape = create_square_shape(cmd.params[0], cmd.params[1], cmd.params[2]);
                    break;
                case RECTANGLE:
                    shape = create_rectangle_shape(cmd.params[0], cmd.params[1], cmd.params[2], cmd.params[3]);
                    break;
                case CIRCLE:
                    shape = create_circle_shape(cmd.params[0], cmd.params[1], cmd.params[2]);
                    break;
                case POLYGON:
                    shape = create_polygon_shape(cmd.params, cmd.num_params);
                    break;
            }
            shapes[shape_count++] = shape;
            printf("La forme a bien été créée\n");
            break;

        case DELETE:
            // Supprimez une forme de la liste de formes.
            for (int i = 0; i < shape_count; i++) {
                if (shapes[i]->id == cmd.params[0]) {
                    delete_shape(shapes[i]);
                    shapes[i] = shapes[--shape_count];
                    printf("la forme ID %d à bien été supprimée\n", shapes[i]->id);              
                    break;
                }
            }
            break;

        case LIST:
            // Affichez toutes les formes dans la liste.
            for (int i = 0; i < shape_count; i++) {
                print_shape(shapes[i]);
            }
            for (int j = 0; j >= shape_count; j ++) {
                printf("\nla LIST est vide !\n");
                break;
            }
            break;

        case CLEAR:
            // Supprimez toutes les formes de la liste.
            for (int j = 0; j >= shape_count; j++) {
                printf("La LIST est déjà vide !\n");
                break;
            }
            for (int i = 0; i < shape_count; i++) {
                delete_shape(shapes[i]);
                printf("Toutes les formes ont bien été supprimées\n");
                break;
            }
            shape_count = 0;
            break;

        case HELP:
            // Afficher toutes les commandes et la façon de les utiliser
            printf("ADD (SHAPE) (COORDINATE) : Créer une forme\n");
            printf("\tex : ADD POINT 3 4\n\n");
            printf("SHAPE : Afficher le nombre de COORDINATE nécessaire pour une forme\n\n");
            printf("LIST : Afficher toutes les formes créées\n\n");
            printf("DELETE (SHAPE) (ID) : Supprimer uniquement la forme sélectionnée dans LIST\n");
            printf("\tex : DELETE CIRCLE 3\n\n");
            printf("CLEAR : Supprimer toutes les formes créées dans LIST\n\n");
            printf("QUIT : Quitter le jeu\n\n");
            break;

        case SHAPE:
            // afficher le nombre de coordonnée nécessaire pour chaque forme 
            printf("POINT [X] [Y] : 2 coordonnées\n");
            printf("CIRCLE [X] [Y] [RADIUS] : 3 coordonnées\n");
            printf("SQUARE [X] [Y] [LENGTH] : 3 coordonnées\n");
            printf("LINE [X1] [Y1] [X2] [Y2] : 4 coordonnées\n");
            printf("RECTANGLE [X] [Y] [WIDTH] [LENGTH] : 4 coordonnées\n");
            break;

        case QUIT:
    // End the program.
    printf("\nMerci d'avoir joué à bientôt :)\n\n");
    exit(0);
    break;
    }
}

int main() {
    char command_str[MAX_COMMAND_LEN];
    Command cmd;
    printf("Pour avoir de l'aide, tapper HELP !");

    set_id(0);

    while (1) {
        printf("\nEnter command: ");
        fgets(command_str, MAX_COMMAND_LEN, stdin);
        cmd = parse_command(command_str);
        execute_command(cmd);
    }

    return 0;
}
