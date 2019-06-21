#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define GAME_TERMINATE -1

//Donuts
#define mint 0
#define strawberry 1
#define white 2
#define random 3

//Direction
#define dx 0
#define dy 1
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

//Pages
#define STARTING_PAGE 1
#define MENU_PAGE 2
#define ABOUT_PAGE 3
#define SELECTION_PAGE 4
#define GAME_PAGE 5
#define PAUSE_PAGE 6
#define RESULT_PAGE 7

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *title_img = NULL;
ALLEGRO_BITMAP *seller = NULL;
ALLEGRO_BITMAP *dialogue = NULL;
ALLEGRO_BITMAP *tutorial[14] = {0};
ALLEGRO_BITMAP *start_text = NULL;
ALLEGRO_BITMAP *menu_plate = NULL;
ALLEGRO_BITMAP *menu_plate_selected = NULL;
ALLEGRO_BITMAP *background_now = NULL;
ALLEGRO_BITMAP *background_blur = NULL;
ALLEGRO_BITMAP *background[3] = {0};
ALLEGRO_BITMAP *about_background = NULL;
ALLEGRO_BITMAP *hp_frame = NULL;
ALLEGRO_BITMAP *deco_1 = NULL;
ALLEGRO_BITMAP *bullet = NULL;
ALLEGRO_BITMAP *crown = NULL;
ALLEGRO_KEYBOARD_STATE keyState ;
ALLEGRO_TIMER *timer0 = NULL;
ALLEGRO_TIMER *timer1 = NULL;
ALLEGRO_TIMER *timer2 = NULL;
ALLEGRO_TIMER *timer3 = NULL;
ALLEGRO_SAMPLE *song1 = NULL;
ALLEGRO_SAMPLE *song2 = NULL;
ALLEGRO_FONT *title_font = NULL;
ALLEGRO_FONT *story_font = NULL;
ALLEGRO_FONT *select_font = NULL;
ALLEGRO_FONT *select_font_2 = NULL;
ALLEGRO_COLOR text_color;

//Custom Definition
//Window & sizes
const char *title = "Final Project 106062361";
const float FPS = 300;
const int WIDTH = 960;
const int HEIGHT = 720;
const int CHARACTER_WIDTH = 200;
const int CHARACTER_HEIGHT = 200;
const int CHARACTER_SRC_WIDTH = 470;
const int CHARACTER_SRC_HEIGHT = 470;
const int BULLET_HEIGHT = 60;
const int HP_LENGTH = 245;
const float radius = 3;
const int plate_width_a = 300;
int plate_width_b[4] = {0};
int plate_height[3] = {0};

//animations
float transparency[5] = {0};
float floating_effect[5] = {0};

//Game physics
const int GRAVITY = 1;
const int BULLET_SPEED = 7;
const int BULLET_DAMAGE = 5;
const int NEAR_DAMAGE = 10;
int sky, land_1, land_2;

//Logic
int ture = 0;
int dialogue_no = 0;
int select_temp = 0;
int selected = 0;
int draw = 0;
int done = 0;
int window = 1;
bool started = 0;
bool paused = 0;
int time_limit = 0;
int game_time = 0;
char time_left_display[4] = {0};
bool judge_next_window = false;

typedef struct _bullet {
    int in_use;
    int x;
    int y;
    int direction;
} BULLET;

typedef struct _character {
    int x;
    int y;
    int height;
    int vy;
    bool jump;
    int attack;
    int hit;
    int direction;
    int walk[2];
    int hp;
    BULLET bullet;
    ALLEGRO_TIMER *skill_cd;
    ALLEGRO_TIMER *bullet_cd;
    ALLEGRO_BITMAP *normal_1_img;
    ALLEGRO_BITMAP *normal_2_img;
    ALLEGRO_BITMAP *hit_img;
    ALLEGRO_BITMAP *attack_img;
    ALLEGRO_BITMAP *jump_img;
    ALLEGRO_BITMAP *lose_img;
    ALLEGRO_BITMAP *now_img;
    ALLEGRO_COLOR color;
} CHARACTER;

CHARACTER *opponent[2]={0};
CHARACTER donut[4]={0};
int winner = 0;

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
void game_destroy();
void game_pause();
void game_continue();
void load_files();
void character_movement(CHARACTER *);
void character_attack(CHARACTER *, CHARACTER *);
void bullet_update(BULLET, CHARACTER *, CHARACTER *);
void to_MENU();
void to_SELECTION();
void to_RESULT();

void load_files()
{
    //mint donut
    donut[mint].normal_1_img = al_load_bitmap("sources/mint-donut_walking-1.png");
    donut[mint].normal_2_img = al_load_bitmap("sources/mint-donut_walking-2.png");
    donut[mint].attack_img = al_load_bitmap("sources/mint-donut_attack.png");
    donut[mint].hit_img = al_load_bitmap("sources/mint-donut_hit.png");
    donut[mint].jump_img = al_load_bitmap("sources/mint-donut_jump.png");
    donut[mint].lose_img = al_load_bitmap("sources/mint-donut_lose.png");
    donut[mint].color = al_map_rgb(140,245,185);

    //strawberry donut
    donut[strawberry].normal_1_img = al_load_bitmap("sources/strawberry-donut_walking-1.png");
    donut[strawberry].normal_2_img = al_load_bitmap("sources/strawberry-donut_walking-2.png");
    donut[strawberry].attack_img = al_load_bitmap("sources/strawberry-donut_attack.png");
    donut[strawberry].hit_img = al_load_bitmap("sources/strawberry-donut_hit.png");
    donut[strawberry].jump_img = al_load_bitmap("sources/strawberry-donut_jump.png");
    donut[strawberry].lose_img = al_load_bitmap("sources/strawberry-donut_lose.png");
    donut[strawberry].color = al_map_rgb(247,170,174);

    //white donut
    donut[white].normal_1_img = al_load_bitmap("sources/white-donut_walking-1.png");
    donut[white].normal_2_img = al_load_bitmap("sources/white-donut_walking-2.png");
    donut[white].attack_img = al_load_bitmap("sources/white-donut_attack.png");
    donut[white].hit_img = al_load_bitmap("sources/white-donut_hit.png");
    donut[white].jump_img = al_load_bitmap("sources/white-donut_jump.png");
    donut[white].lose_img = al_load_bitmap("sources/white-donut_lose.png");
    donut[white].color = al_map_rgb(254,254,242);

    //background
    background[0] = al_load_bitmap("sources/background-1.jpg");
    background[1] = al_load_bitmap("sources/background-2.jpg");
    background_blur = al_load_bitmap("sources/background-blur.jpg");
    about_background = al_load_bitmap("sources/about.png");
    hp_frame = al_load_bitmap("sources/hp_frame.png");

    //menu
    seller = al_load_bitmap("sources/Seller.png");
    dialogue = al_load_bitmap("sources/dialogue.png");
    start_text = al_load_bitmap("sources/start-text.png");
    seller = al_load_bitmap("sources/Seller.png");
    deco_1 = al_load_bitmap("sources/deco1.png");
    title_img = al_load_bitmap("sources/title.png");
    menu_plate = al_load_bitmap("sources/menu-plate.png");
    menu_plate_selected = al_load_bitmap("sources/menu-plate_selected.png");
    story_font = al_load_ttf_font("sources/cute.ttf",28,0);
    title_font = al_load_ttf_font("sources/menu.ttf",40,0);
    select_font = al_load_ttf_font("sources/menu.ttf",30,0);
    select_font_2 = al_load_ttf_font("sources/cute.ttf",40,0);
    text_color = al_map_rgb(94,76,51);

    //tutorial
    tutorial[0] = al_load_bitmap("sources/tutorial-p1_all.png");
    tutorial[1] = al_load_bitmap("sources/tutorial-p2_all.png");
    tutorial[2] = al_load_bitmap("sources/tutorial-p1_jump.png");
    tutorial[3] = al_load_bitmap("sources/tutorial-p1_up.png");
    tutorial[4] = al_load_bitmap("sources/tutorial-p1_attack.png");
    tutorial[5] = al_load_bitmap("sources/tutorial-p1_left.png");
    tutorial[6] = al_load_bitmap("sources/tutorial-p1_down.png");
    tutorial[7] = al_load_bitmap("sources/tutorial-p1_right.png");
    tutorial[8] = al_load_bitmap("sources/tutorial-p2_jump.png");
    tutorial[9] = al_load_bitmap("sources/tutorial-p2_up.png");
    tutorial[10] = al_load_bitmap("sources/tutorial-p2_attack.png");
    tutorial[11] = al_load_bitmap("sources/tutorial-p2_left.png");
    tutorial[12] = al_load_bitmap("sources/tutorial-p2_down.png");
    tutorial[13] = al_load_bitmap("sources/tutorial-p2_right.png");

    //bullet
    bullet = al_load_bitmap("sources/bullet.png");

    //crown
    crown = al_load_bitmap("sources/crown.png");

    //songs
    song1 = al_load_sample("sources/moco ani - Light up my LOVE!!.wav");
    song2 = al_load_sample("sources/hello.wav");
    if (!song2||!song1){
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-1);
    }

}

void character_movement(CHARACTER *character)
{
    //walk
    if((character->x >= 0 && character->walk[dx] == LEFT) || (character->x <= WIDTH-CHARACTER_WIDTH && character->walk[dx]== RIGHT))
        character->x += character->walk[dx]*5;
    if((character->y >= land_1  && character->walk[dy] == UP) || (character->y <= land_2 && character->walk[dy]== DOWN))
        character->y += character->walk[dy]*5;

    //jump
    if(character->y >= sky && character->jump)
    {
        character->vy += GRAVITY;
        character->height += character->vy;
        if(character->height > 0) {
            character->jump = false;
            character->vy = -25;
            character->height = 0;
        }
    }
        //character->skill_cd = al_create_timer(3);
        //al_start_timer(character->skill_cd);
}

void character_attack(CHARACTER *attacker, CHARACTER *victim)
{
    if(attacker->y+attacker->height >= victim-> y+victim->height-30 && attacker->y+attacker->height <= victim-> y+victim->height+30){
        if(attacker->x>victim->x && attacker->direction==LEFT){
            if(victim->direction==LEFT && attacker->x<=victim->x+CHARACTER_WIDTH+50) attacker->attack = 2;
            else if(victim->direction==RIGHT && attacker->x<=victim->x+CHARACTER_WIDTH+50) attacker->attack = 2;
        }
        else if(attacker->x<victim->x && attacker->direction==RIGHT) {
            if(victim->direction==RIGHT && attacker->x>=victim->x-CHARACTER_WIDTH-50) attacker->attack = 2;
            else if(victim->direction==LEFT && attacker->x>=victim->x-CHARACTER_WIDTH-50) attacker->attack = 2;
        }
    }
    if(attacker->attack==2)
    {
        victim->hp -= NEAR_DAMAGE;
        victim->hit = 2;
    }
    else
    {
        attacker->attack = 1;
        if(attacker->bullet.in_use==0)
        {
            attacker->bullet.in_use = 1;
            attacker->bullet.y = attacker->y + BULLET_HEIGHT;
            attacker->bullet.direction = attacker->direction;
            if(attacker->direction==RIGHT) attacker->bullet.x = attacker->x + CHARACTER_WIDTH;
            if(attacker->direction==LEFT) attacker->bullet.x = attacker->x;
        }
    }
}

void game_pause()
{
    window = PAUSE_PAGE;
    al_stop_timer(timer3);
    al_stop_samples();
    selected = 0;
    floating_effect[0] = 1500;
    //al_flush_event_queue(event_queue);
}

void game_continue()
{
    window = GAME_PAGE;
    timer3 = al_create_timer(1.0);
    al_start_timer(timer3);
    al_register_event_source(event_queue, al_get_timer_event_source(timer3)) ;
    al_stop_samples();
    al_play_sample(song2, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
}

void to_MENU()
{
    window = MENU_PAGE;
    selected = 0;
    dialogue_no = 0;
    transparency[0] = 0;
    floating_effect[0] = 1500;
}

void to_SELECTION()
{
    window = SELECTION_PAGE;
    timer3 = al_create_timer(1.0);
    al_start_timer(timer3);
    al_register_event_source(event_queue, al_get_timer_event_source(timer3)) ;
    selected = 0;
    floating_effect[0] = 1500;
    opponent[0] = NULL;
    opponent[1] = NULL;
    started = 0;
    //load_files();
}

void to_RESULT()
{
    window = RESULT_PAGE;
    selected = 0;
    transparency[0] = 0;
    floating_effect[0] = 1000;
    floating_effect[1] = 1500;
    if(opponent[0]->hp>opponent[1]->hp)
        winner = 0;
    else if(opponent[0]->hp<opponent[1]->hp)
        winner = 1;
    else winner = 2;
}
int main(int argc, char *argv[]) {
    int msg = 0;

    game_init();
    game_begin();
    //load_files();

    while (msg != GAME_TERMINATE) {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
    }

    game_destroy();
    return 0;
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init() {
    if (!al_init()) {
        show_err_msg(-1);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-1);
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-1);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-1);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL) {
        show_err_msg(-1);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    load_files();

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void game_begin() {
    al_stop_samples();
    // Loop the song until the display closes
    al_play_sample(song1, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

    // Load and draw text
    al_draw_bitmap(background[0], 0,0, 0);
    al_draw_tinted_bitmap(background_blur, al_map_rgba_f(1,1,1,transparency[0]), 0,0, 0);
    al_draw_rectangle(200, 510, 600, 550, al_map_rgb(255, 255, 255), 0);
    al_flip_display();

    //Initialize Timer
    //al_stop_timer(timer0);
    timer1  = al_create_timer(0.02);
    timer2  = al_create_timer(1.0/3.0);
    al_register_event_source(event_queue, al_get_timer_event_source(timer1)) ;
    al_register_event_source(event_queue, al_get_timer_event_source(timer2)) ;
    al_start_timer(timer1);
    al_start_timer(timer2);

    transparency[0] = 0;
    floating_effect[0] = -1000;
}

int process_event(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    // Our setting for controlling animation
    if(event.timer.source == timer2){
            ture = !ture;
    }

    // Keyboard
    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(event.keyboard.keycode)
        {
            // P1 control
            case ALLEGRO_KEY_Q:
                if(window==GAME_PAGE){
                    opponent[0]->jump = true;
                    opponent[0]->hit = 0;
                    if(opponent[0]->attack==2) opponent[0]->attack = 0;
                }
                break;
            case ALLEGRO_KEY_W:
                if(window==STARTING_PAGE)
                {
                    if(dialogue_no) dialogue_no--;
                }
                else if(window == GAME_PAGE) {
                    opponent[0]->walk[dy] = UP;
                    opponent[0]->hit = 0;
                    if(opponent[0]->attack==2) opponent[0]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+2)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                    {
                        selected = (selected+2)%3;
                        if(selected==select_temp && opponent[0])
                            selected = (selected+2)%3;
                    }
                    else time_limit = (time_limit+2)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+2)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_S:
                if(window==STARTING_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==5) to_MENU();
                }
                if(window==ABOUT_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==12) to_MENU();
                }
                else if(window == GAME_PAGE) {
                    opponent[0]->walk[dy] = DOWN;
                    opponent[0]->hit = 0;
                    if(opponent[0]->attack==2) opponent[0]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+1)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                        selected = (selected+1)%3;
                    else time_limit = (time_limit+1)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+2)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_A:
                if(window==STARTING_PAGE||window==STARTING_PAGE)
                {
                    if(dialogue_no) dialogue_no--;
                }
                if(window == GAME_PAGE) {
                    opponent[0]->walk[dx] = LEFT;
                    opponent[0]->direction = LEFT;
                    opponent[0]->hit = 0;
                    if(opponent[0]->attack==2) opponent[0]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+2)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                    {
                        selected = (selected+2)%3;
                        if(selected==select_temp && opponent[0])
                            selected = (selected+2)%3;
                    }
                    else time_limit = (time_limit+2)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+3)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_D:
                if(window==STARTING_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==5) to_MENU();
                }
                if(window==ABOUT_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==12) to_MENU();
                }
                if(window == GAME_PAGE) {
                    opponent[0]->walk[dx] = RIGHT;
                    opponent[0]->direction = RIGHT;
                    opponent[0]->hit = 0;
                    if(opponent[0]->attack==2) opponent[0]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+1)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                    {
                        selected = (selected+1)%3;
                        if(selected==select_temp && opponent[0])
                            selected = (selected+1)%3;
                    }
                    else time_limit = (time_limit+1)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+1)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_E:
                if(window == GAME_PAGE) {
                    if(opponent[0]->attack==2) opponent[0]->attack = 0;
                    if(opponent[0]->bullet.in_use==0)
                        character_attack(opponent[0], opponent[1]);
                    opponent[0]->hit = 0;
                }
                break;

            // P2 control
            case ALLEGRO_KEY_RSHIFT:
                if(window==GAME_PAGE){
                    opponent[1]->jump = true;
                    opponent[1]->hit = 0;
                    if(opponent[1]->attack==2) opponent[1]->attack = 0;
                }
                break;
            case ALLEGRO_KEY_UP:
                if(window==STARTING_PAGE)
                {
                    if(dialogue_no) dialogue_no--;
                }
                else if(window == GAME_PAGE) {
                    opponent[1]->walk[dy] = UP;
                    opponent[1]->hit = 0;
                    if(opponent[1]->attack==2) opponent[1]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+2)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                    {
                        selected = (selected+2)%3;
                        if(selected==select_temp && opponent[0])
                            selected = (selected+2)%3;
                    }
                    else time_limit = (time_limit+2)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+2)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_DOWN:
                if(window==STARTING_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==5) to_MENU();
                }
                if(window==ABOUT_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==12) to_MENU();
                }
                else if(window == GAME_PAGE) {
                    opponent[1]->walk[dy] = DOWN;
                    opponent[1]->hit = 0;
                    if(opponent[1]->attack==2) opponent[1]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+1)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                        selected = (selected+1)%3;
                    else time_limit = (time_limit+1)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+2)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_LEFT:
                if(window==STARTING_PAGE||window==STARTING_PAGE)
                {
                    if(dialogue_no) dialogue_no--;
                }
                if(window == GAME_PAGE) {
                    opponent[1]->walk[dx] = LEFT;
                    opponent[1]->direction = LEFT;
                    opponent[1]->hit = 0;
                    if(opponent[1]->attack==2) opponent[1]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+2)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                    {
                        selected = (selected+2)%3;
                        if(selected==select_temp && opponent[0])
                            selected = (selected+2)%3;
                    }
                    else time_limit = (time_limit+2)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+3)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;
            case ALLEGRO_KEY_RIGHT:
                if(window==STARTING_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==5) to_MENU();
                }
                if(window==ABOUT_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==12) to_MENU();
                }
                if(window == GAME_PAGE) {
                    opponent[1]->walk[dx] = RIGHT;
                    opponent[1]->direction = RIGHT;
                    opponent[1]->hit = 0;
                    if(opponent[1]->attack==2) opponent[1]->attack = 0;
                }
                else if(window == MENU_PAGE)
                    selected = (selected+1)%3;
                else if(window == SELECTION_PAGE)
                {
                    if(opponent[0]==NULL||opponent[1]==NULL)
                    {
                        selected = (selected+1)%3;
                        if(selected==select_temp && opponent[0])
                            selected = (selected+1)%3;
                    }
                    else time_limit = (time_limit+1)%3;
                }
                else if(window == PAUSE_PAGE)
                    selected = (selected+1)%4;
                else if(window == RESULT_PAGE)
                    selected = (selected+1)%2;
                break;

            case ALLEGRO_KEY_RCTRL:
                if(window == GAME_PAGE) {
                    if(opponent[1]->attack==2) opponent[1]->attack = 0;
                    if(opponent[1]->bullet.in_use==0)
                        character_attack(opponent[1], opponent[0]);
                    opponent[1]->hit = 0;
                }
                break;

            // For Menu
            case ALLEGRO_KEY_ENTER:
                if(window==STARTING_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==5) to_MENU();
                }
                if(window==ABOUT_PAGE)
                {
                    dialogue_no++;
                    if(dialogue_no==12) to_MENU();
                }
                else if(window==MENU_PAGE)
                {
                    if(selected==0) to_SELECTION();
                    if(selected==1)
                    {
                        window = ABOUT_PAGE;
                        dialogue_no = 0;
                        floating_effect[0] = 1500;
                    }
                    if(selected==2) return GAME_TERMINATE;
                }
                else if(window==SELECTION_PAGE)
                {
                    if(opponent[1]!=NULL) {
                        time_limit *= 60;
                        game_time = time_limit;
                        window = GAME_PAGE;
                    }
                    else if(opponent[0]==NULL){
                        opponent[0] = &donut[selected];
                        select_temp = selected;
                        selected=(selected+1)%3;
                    }
                    else {
                        opponent[1] = &donut[selected];
                        time_limit = 0;
                    }
                }
                else if(window==PAUSE_PAGE)
                {
                    al_play_sample(song1, 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
                    if(selected==0) game_continue();
                    else if(selected==1) to_SELECTION();
                    else if(selected==2) to_MENU();
                    else if(selected==3) return GAME_TERMINATE;
                }
                else if(window==RESULT_PAGE)
                {
                    if(selected) return GAME_TERMINATE;
                    else to_MENU();
                }
                break;

            case ALLEGRO_KEY_BACKSPACE:
                if(window==MENU_PAGE) window = STARTING_PAGE;
                else if(window==GAME_PAGE) to_SELECTION();
                else if(window==STARTING_PAGE) game_begin();
                else to_MENU();
                break;

            case ALLEGRO_KEY_SPACE:
                if(window==GAME_PAGE && !paused) game_pause();
                else if(window==PAUSE_PAGE) game_continue();
        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(event.keyboard.keycode)
        {
            // P1 control
            case ALLEGRO_KEY_W:
                if(window==GAME_PAGE && started==1)
                    opponent[0]->walk[dy] = 0;
                break;
            case ALLEGRO_KEY_S:
                if(window==GAME_PAGE && started==1)
                    opponent[0]->walk[dy] = 0;
                break;
            case ALLEGRO_KEY_A:
                if(window==GAME_PAGE && started==1)
                    opponent[0]->walk[dx] = 0;
                break;
            case ALLEGRO_KEY_D:
                if(window==GAME_PAGE && started==1)
                    opponent[0]->walk[dx] = 0;
                break;

            // P2 control
            case ALLEGRO_KEY_UP:
                if(window==GAME_PAGE && started==1)
                    opponent[1]->walk[dy] = 0;
                break;
            case ALLEGRO_KEY_DOWN:
                if(window==GAME_PAGE && started==1)
                    opponent[1]->walk[dy] = 0;
                break;
            case ALLEGRO_KEY_RIGHT:
                if(window==GAME_PAGE && started==1)
                opponent[1]->walk[dx] = 0;
                break;
            case ALLEGRO_KEY_LEFT:
                if(window==GAME_PAGE && started==1)
                opponent[1]->walk[dx] = 0;
                break;
        }
    }

    if(window==GAME_PAGE && started==1)
    {
        for(int i=0; i<=1; i++)
        {
            character_movement(opponent[i]);
            //bullet
            if(opponent[i]->bullet.in_use)
            {
                //bullet_update(opponent[i]->bullet, opponent[i], opponent[!i]);
                opponent[i]->bullet.x += opponent[i]->bullet.direction *BULLET_SPEED;
                //if(((opponent[i]->bullet.direction==LEFT && opponent[i]->x>opponent[!i]->x && )||(opponent[i]->bullet.direction==RIGHT && opponent[i]->x<=opponent[!i]->x && opponent[i]->bullet.x>=opponent[!i]->x+30 && opponent[i]->bullet.x<=opponent[i])) && opponent[i]->bullet.y>opponent[!i]->y+opponent[!i]->height+50 && opponent[i]->bullet.y<opponent[!i]->y+opponent[!i]->height+CHARACTER_HEIGHT-50)
                bool boom=0;
                if(opponent[i]->bullet.y >= opponent[!i]->y+opponent[!i]->height+10 && opponent[i]->bullet.y <= opponent[!i]->y+opponent[!i]->height+CHARACTER_WIDTH+10)
                {
                    if(opponent[i]->bullet.x>=opponent[!i]->x+10 && opponent[i]->bullet.x<=opponent[!i]->x+CHARACTER_WIDTH-10){
                        opponent[i]->bullet.in_use = 0;
                        opponent[i]->attack = 0;
                        opponent[!i]->hp -= BULLET_DAMAGE;
                        opponent[!i]->hit = 1;
                    }
                }
                if(opponent[i]->bullet.x<0||opponent[i]->bullet.x>WIDTH)
                {
                    opponent[i]->bullet.in_use = 0;
                    opponent[i]->attack = 0;
                }
            }
        }
        if(opponent[0]->hp<=0||opponent[1]->hp<=0)
            to_RESULT();

        // count game time
        if(event.timer.source == timer3)
            game_time--;

        if(time_limit&&game_time<0)
            to_RESULT();
        else{
            for(int i=0, a=100; i<3; i++, a/=10)
                time_left_display[i] = (int)(game_time/a)%10 + '0';
            time_left_display[3] = '\0';
        }
    }

    // Shutdown our program
    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;

    return 0;
}

int game_run() {
    int error = 0;
    // First window(Menu)
    if(window == STARTING_PAGE)
    {
        al_draw_bitmap(background_blur, 0,0, 0);
        al_draw_tinted_bitmap(background[0], al_map_rgba_f(1,1,1,transparency[0]), 0, 0, 0);
        al_draw_bitmap(title_img, 0, floating_effect[0], 0);
        al_draw_tinted_bitmap(seller, al_map_rgba_f(1,1,1,transparency[0]), 0, 0, 0);
        al_draw_tinted_bitmap(dialogue, al_map_rgba_f(1,1,1,transparency[0]), 0, 0, 0);
        if(dialogue_no==0) al_draw_tinted_bitmap(start_text, al_map_rgba_f(1,1,1,transparency[0]), 0, 0, 0);
        if(transparency[0]<=0.997) transparency[0]+=0.003;
        if(floating_effect[0]<0) floating_effect[0] += 2;

        if(dialogue_no==1){
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-40,ALLEGRO_ALIGN_CENTER,"Welcome to Donut Empire! I am the donut");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-10,ALLEGRO_ALIGN_CENTER,"seller Lily. ");
        }
        if(dialogue_no==2){
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-40,ALLEGRO_ALIGN_CENTER,"Donut Empire used to be an ordinary donut");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-10,ALLEGRO_ALIGN_CENTER,"vendor. However, yesterday I got some");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4+20,ALLEGRO_ALIGN_CENTER,"special secret ingredients from an old");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4+50,ALLEGRO_ALIGN_CENTER,"woman at a market, which claimed to");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4+80,ALLEGRO_ALIGN_CENTER,"make my donuts more delicious.");
        }
        if(dialogue_no==3){
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-40,ALLEGRO_ALIGN_CENTER,"I follow the instructions on the bottle,");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-10,ALLEGRO_ALIGN_CENTER,"and things turned out miraculously -");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4+20,ALLEGRO_ALIGN_CENTER,"the donuts suddenly CAME ALIVE!!");
        }
        if(dialogue_no==4){
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-40,ALLEGRO_ALIGN_CENTER,"Those donuts are cute but aggressive.");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4-10,ALLEGRO_ALIGN_CENTER,"They fight with each other for the throne.");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4+20,ALLEGRO_ALIGN_CENTER,"The final winner will be DONUT KING who");
            al_draw_text(story_font,text_color,WIDTH*2/3+12, HEIGHT*3/4+50,ALLEGRO_ALIGN_CENTER,"role the entire donut empire!");
        }
        al_flip_display();
        al_clear_to_color(al_map_rgb(1,1,1));

        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
        }
        //timer0  = al_create_timer(0.01);
        //al_start_timer(timer0);
    }
    else if(window == MENU_PAGE)
    {
        //timer0  = al_create_timer(0.01);
        //al_start_timer(timer0);
        al_draw_bitmap(background[0], 0,0, 0);
        al_draw_tinted_bitmap(background_blur, al_map_rgba_f(1,1,1,transparency[0]), 0, 0, 0);
        if(transparency[0]<=0.995) transparency[0]+=0.005;

        plate_height[0] = 200+floating_effect[0];
        plate_height[1] = plate_height[0]+150;
        plate_height[2] = plate_height[1]+150;
        for(int i=0; i<3; i++)
            if(i!=selected) al_draw_scaled_bitmap(menu_plate, 0, 0, al_get_bitmap_width(menu_plate), al_get_bitmap_height(menu_plate), (float)WIDTH*3/8, plate_height[i], (float)WIDTH*1/4,(float)WIDTH*1/4*al_get_bitmap_height(menu_plate)/al_get_bitmap_width(menu_plate),0);
        al_draw_scaled_bitmap(menu_plate_selected, 0, 0, al_get_bitmap_width(menu_plate_selected), al_get_bitmap_height(menu_plate_selected), (float)WIDTH*3/8, plate_height[selected], (float)WIDTH*1/4, (float)WIDTH*1/4*al_get_bitmap_height(menu_plate_selected)/al_get_bitmap_width(menu_plate_selected) ,0);
        if(floating_effect[0]>0) floating_effect[0] -= 3;

        al_draw_text(title_font,text_color, WIDTH/2, plate_height[0]+10 ,ALLEGRO_ALIGN_CENTER,"Start");
        al_draw_text(title_font,text_color, WIDTH/2, plate_height[1]+10 ,ALLEGRO_ALIGN_CENTER,"About");
        al_draw_text(title_font,text_color, WIDTH/2, plate_height[2]+10 ,ALLEGRO_ALIGN_CENTER,"Exit");


        al_flip_display();
        al_clear_to_color(al_map_rgb(1,1,1));

        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }
    else if(window == SELECTION_PAGE)
    {
        //timer0  = al_create_timer(0.001);
        //al_start_timer(timer0);
        al_draw_bitmap(background_blur, 0, 0, 0);
        al_draw_bitmap(about_background, 0, floating_effect[0]-60, 0);
        al_draw_text(select_font,text_color, WIDTH/2, HEIGHT/6+floating_effect[0] ,ALLEGRO_ALIGN_CENTER,"Pick Character and Time Limit");
        al_draw_scaled_bitmap(deco_1, 0, 0, al_get_bitmap_width(deco_1), al_get_bitmap_height(deco_1), WIDTH*3/8,HEIGHT/6+50+floating_effect[0], WIDTH/4, HEIGHT/20, 0);

        plate_height[0] = 240+floating_effect[0];
        plate_height[1] = plate_height[0]+300;
        plate_width_b[0] = 170;
        plate_width_b[1] = plate_width_b[0]+CHARACTER_WIDTH+25;
        plate_width_b[2] = plate_width_b[1]+CHARACTER_WIDTH+25;
        plate_width_b[3] = plate_width_b[2]+CHARACTER_WIDTH+25;

        //donut[random].normal_1_img = al_load_bitmap("sources/random-donut_normal.png");
        //donut[random].normal_2_img = al_load_bitmap("sources/random-donut_big.png");

        for(int i=0; i<3; i++){
            if(i!=selected&&(i!=select_temp||!opponent[0])) al_draw_scaled_bitmap(donut[i].normal_2_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, plate_width_b[i], plate_height[0], CHARACTER_WIDTH, CHARACTER_HEIGHT, 0);
            al_draw_bitmap(menu_plate, plate_width_b[i], plate_height[1], 0);
        }
        if(opponent[0]) al_draw_scaled_bitmap(donut[select_temp].jump_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, plate_width_b[select_temp]-20, plate_height[0]-20, CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3, 0);
        al_draw_scaled_bitmap(donut[selected].jump_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, plate_width_b[selected]-20, plate_height[0]-20, CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3, 0);
        if(opponent[1]) al_draw_bitmap(menu_plate_selected, plate_width_b[time_limit], plate_height[1], 0);

        al_draw_text(select_font,text_color, WIDTH/4+10, plate_height[1]+7 ,ALLEGRO_ALIGN_CENTER,"no");
        al_draw_text(select_font,text_color, WIDTH/2, plate_height[1]+7 ,ALLEGRO_ALIGN_CENTER,"1 min");
        al_draw_text(select_font,text_color, WIDTH*3/4-5, plate_height[1]+7 ,ALLEGRO_ALIGN_CENTER,"2 min");

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        if(floating_effect[0]>0) floating_effect[0] -= 3;

        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }
    else if(window == ABOUT_PAGE)
    {
        al_draw_bitmap(background_blur, 0, 0, 0);
        al_draw_bitmap(about_background, 0, 0, 0);
        al_draw_text(title_font,text_color, WIDTH/2, HEIGHT/4 ,ALLEGRO_ALIGN_CENTER,"Tutorial");
        al_draw_scaled_bitmap(deco_1, 0, 0, al_get_bitmap_width(deco_1), al_get_bitmap_height(deco_1), WIDTH*3/8,HEIGHT/3.1, WIDTH/4, HEIGHT/20, 0);
        al_draw_bitmap(seller, 0, 0, 0);
        if(dialogue_no<=5) al_draw_bitmap(tutorial[0], 0, 0, 0);
        else al_draw_bitmap(tutorial[1], 0, 0, 0);
        //al_draw_justified_text(title_font,text_color, WIDTH/2, HEIGHT/4 ,ALLEGRO_ALIGN_CENTER,"About");
        for(int i=2; i<14; i++)
        {
            if(dialogue_no+2==i) al_draw_bitmap(tutorial[i], 0, 0, 0);
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }
    // Main Game
    else if(window == GAME_PAGE)
    {
        if(!started)
        {
            started = 1;

            //play song
            al_stop_samples();
            al_play_sample(song2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP,NULL);

            // Initialize Game
            sky = HEIGHT/3;
            land_1 = HEIGHT*1/2;
            land_2 = HEIGHT*2/3;

            for(int i=0; i<=1; i++)
            {
                opponent[i]->walk[i+1] = 1;
                opponent[i]->y = (land_1+land_2)/2;
                opponent[i]->walk[dx] = 0;
                opponent[i]->walk[dy] = 0;
                opponent[i]->now_img = opponent[i]->normal_1_img;
                opponent[i]->vy = -25;
                opponent[i]->height = 0;
                opponent[i]->jump = 0;
                opponent[i]->attack = 0;
                opponent[i]->hit = 0;
                opponent[i]->bullet.in_use = 0;
                opponent[i]->hp = 100;
            }
            opponent[0]->x = 100;
            opponent[0]->direction = RIGHT;
            opponent[1]->x = WIDTH-100-CHARACTER_WIDTH;
            opponent[1]->direction = LEFT;

            //background
            background_now = background[0];
        }

        //continue game
        al_draw_bitmap(background_now, 0,0, 0);
        if(time_limit&&game_time>0) al_draw_tinted_bitmap(background[1], al_map_rgba_f(1,1,1,(float)(time_limit-game_time)/time_limit), 0, 0, 0);
        //al_draw_bitmap(al_load_bitmap("sources/hp_base.png"), 0, 0, 0);

        //time & hp
        al_draw_filled_rectangle(120, 41, 120+HP_LENGTH, 60, text_color);
        al_draw_filled_rectangle(840-HP_LENGTH, 41, 840, 60, text_color);
        al_draw_filled_rectangle(120, 41, 120+(float)(opponent[0]->hp)/100*HP_LENGTH, 60, opponent[0]->color);
        al_draw_filled_rectangle(840-(float)(opponent[1]->hp)/100*HP_LENGTH, 41, 840, 60, opponent[1]->color);
        al_draw_bitmap(hp_frame, 0, 0, 0);
        if(time_limit) al_draw_text(title_font,text_color, WIDTH/2, 25 ,ALLEGRO_ALIGN_CENTER, time_left_display);

        //judge who is in front
        int i;
        if(opponent[0]->y>opponent[1]->y) i=1;
        else i=0;
        for(int j=0; j<=1; j++, i=!i)
        {
            // Change Image for animation
            if(opponent[i]->hit==1) opponent[i]->now_img = opponent[i]->hit_img;
            else if(opponent[i]->hit==2) opponent[i]->now_img = opponent[i]->lose_img;
            else if(opponent[i]->jump) opponent[i]->now_img = opponent[i]->jump_img;
            else if(opponent[i]->attack) opponent[i]->now_img = opponent[i]->attack_img;
            else if(ture) opponent[i]->now_img = opponent[i]->normal_1_img;
            else opponent[i]->now_img = opponent[i]->normal_2_img;

            if(opponent[i]->direction==LEFT) al_draw_scaled_bitmap(opponent[i]->now_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, opponent[i]->x, opponent[i]->y+opponent[i]->height, CHARACTER_WIDTH*opponent[i]->y*2/(land_1+land_2), CHARACTER_HEIGHT*opponent[i]->y*2/(land_1+land_2), 0);
            else al_draw_scaled_bitmap(opponent[i]->now_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, opponent[i]->x+CHARACTER_WIDTH, opponent[i]->y+opponent[i]->height, -CHARACTER_WIDTH*opponent[i]->y*2/(land_1+land_2), CHARACTER_HEIGHT*opponent[i]->y*2/(land_1+land_2), 0);

            //draw bullet
            if(opponent[i]->bullet.in_use)
                al_draw_scaled_bitmap(bullet, 0, 0, al_get_bitmap_width(bullet), al_get_bitmap_height(bullet), opponent[i]->bullet.x, opponent[i]->bullet.y+opponent[i]->height, 40, 30,0);

            /*for(int j=0; j<=30; j++)
            {
                if(opponent[i]->bullet[j].in_use)
                    al_draw_bitmap(al_load_bitmap("sources/bullet.png"), opponent[i]->bullet[j].x, opponent[i]->bullet[j].y, 0);
            }*/
        }
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        // Listening for new event
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
        }
    }
    //Pause Page
    else if(window == PAUSE_PAGE)
    {
        //timer0  = al_create_timer(0.01);
        //al_start_timer(timer0);
        al_draw_bitmap(background[0], 0,0, 0);
        al_draw_tinted_bitmap(background_blur, al_map_rgba_f(1,1,1,transparency[0]), 0, 0, 0);
        if(transparency[0]<=0.99) transparency[0]+=0.01;

        plate_height[0] = HEIGHT*3/7+floating_effect[0];
        plate_height[1] = plate_height[0]+120;
        plate_width_b[0] = al_get_bitmap_width(menu_plate)+50;
        plate_width_b[1] = WIDTH-al_get_bitmap_width(menu_plate)*2.15-50;

        al_draw_bitmap(about_background, 0, floating_effect[0], 0);
        for(int i=0; i<4; i++)
            if(i!=selected) al_draw_scaled_bitmap(menu_plate, 0, 0, al_get_bitmap_width(menu_plate), al_get_bitmap_height(menu_plate), plate_width_b[i%2], plate_height[i/2],al_get_bitmap_width(menu_plate)*1.15, al_get_bitmap_height(menu_plate)*1.15,0);
        al_draw_scaled_bitmap(menu_plate_selected, 0, 0, al_get_bitmap_width(menu_plate), al_get_bitmap_height(menu_plate), plate_width_b[selected%2], plate_height[selected/2],al_get_bitmap_width(menu_plate)*1.15, al_get_bitmap_height(menu_plate)*1.15,0);

        al_draw_text(title_font,text_color, WIDTH/2, HEIGHT/4+floating_effect[0], ALLEGRO_ALIGN_CENTER,"Paused");
        al_draw_scaled_bitmap(deco_1, 0, 0, al_get_bitmap_width(deco_1), al_get_bitmap_height(deco_1), WIDTH*3/8,HEIGHT/3.1+floating_effect[0], WIDTH/4, HEIGHT/20, 0);
        //al_draw_bitmap(seller, 0, floating_effect[0], 0);

        if(floating_effect[0]>0) floating_effect[0] -= 3;

        al_draw_text(select_font_2,text_color, plate_width_b[0]+al_get_bitmap_width(menu_plate)*1.15/2, plate_height[0]+15 ,ALLEGRO_ALIGN_CENTER,"Continue");
        al_draw_text(select_font_2,text_color, (WIDTH-al_get_bitmap_width(menu_plate)*1.15+plate_width_b[1])/2, plate_height[0]+15 ,ALLEGRO_ALIGN_CENTER,"Retry");
        al_draw_text(select_font_2,text_color, plate_width_b[0]+al_get_bitmap_width(menu_plate)*1.15/2, plate_height[1]+15 ,ALLEGRO_ALIGN_CENTER,"Menu");
        al_draw_text(select_font_2,text_color, (WIDTH-al_get_bitmap_width(menu_plate)*1.15+plate_width_b[1])/2, plate_height[1]+15 ,ALLEGRO_ALIGN_CENTER,"Exit");


        al_flip_display();
        al_clear_to_color(al_map_rgb(1,1,1));

        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }
    //Result Page
    else if(window == RESULT_PAGE)
    {
        if(started)
        {
            al_drop_next_event(event_queue);
            started = 0;
            al_stop_samples();
            al_play_sample(song1, 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
        }
        al_draw_bitmap(background[0], 0, 0, 0);
        al_draw_tinted_bitmap(background_blur, al_map_rgba_f(1,1,1,transparency[0]), 0,0, 0);
        al_draw_bitmap(about_background, 0, floating_effect[0]-60, 0);
        al_draw_text(title_font,text_color, WIDTH/2, HEIGHT/5+floating_effect[0] ,ALLEGRO_ALIGN_CENTER,"Game Over");
        al_draw_scaled_bitmap(deco_1, 0, 0, al_get_bitmap_width(deco_1), al_get_bitmap_height(deco_1), WIDTH*3/8,HEIGHT/3.5+floating_effect[0], WIDTH/4, HEIGHT/20, 0);


        if(transparency[0]<1) transparency[0] += 0.2;
        if(winner==1)
        {
            if(ture) {
                al_draw_scaled_bitmap(opponent[0]->lose_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)/3+CHARACTER_WIDTH*1.3+50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], -CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(opponent[1]->jump_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)*2/3+CHARACTER_WIDTH*1.3-50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_bitmap(crown, -30, floating_effect[0], 0);
            }
            else {
                al_draw_scaled_bitmap(opponent[0]->hit_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)/3+CHARACTER_WIDTH*1.3+50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], -CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(opponent[1]->attack_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)*2/3+CHARACTER_WIDTH*1.3-50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
            }
        }
        else if(winner==0)
        {
            if(ture) {
                al_draw_scaled_bitmap(opponent[0]->jump_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)/3+CHARACTER_WIDTH*1.3+50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], -CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(opponent[1]->lose_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)*2/3+CHARACTER_WIDTH*1.3-50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(crown, 0, 0, al_get_bitmap_width(crown), al_get_bitmap_height(crown), al_get_bitmap_width(crown)+30, floating_effect[0], -al_get_bitmap_width(crown), al_get_bitmap_height(crown), 0);
            }
            else {
                al_draw_scaled_bitmap(opponent[0]->attack_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)/3+CHARACTER_WIDTH*1.3+50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], -CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(opponent[1]->hit_img, 0, 0, CHARACTER_SRC_WIDTH+30, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)*2/3+CHARACTER_WIDTH*1.3-50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
            }
        }
        else
        {
            if(ture) {
                al_draw_scaled_bitmap(opponent[ture]->hit_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)/3+CHARACTER_WIDTH*1.3+50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], -CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(opponent[!ture]->attack_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)*2/3+CHARACTER_WIDTH*1.3-50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
            }
            else {
                al_draw_scaled_bitmap(opponent[!ture]->attack_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)/3+CHARACTER_WIDTH*1.3+50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], -CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
                al_draw_scaled_bitmap(opponent[ture]->hit_img, 0, 0, CHARACTER_SRC_WIDTH, CHARACTER_SRC_HEIGHT, (WIDTH-2*CHARACTER_WIDTH*1.3)*2/3+CHARACTER_WIDTH*1.3-50,(HEIGHT-CHARACTER_HEIGHT)/2+floating_effect[0], CHARACTER_WIDTH*1.3, CHARACTER_HEIGHT*1.3,0);
            }
        }
        plate_height[0] = 560 + floating_effect[1];
        plate_width_b[0] = al_get_bitmap_width(menu_plate)-10;
        plate_width_b[1] = WIDTH-al_get_bitmap_width(menu_plate)-al_get_bitmap_width(menu_plate)*1.2+10;

        for(int i=0; i<2; i++)
            if(i!=selected) al_draw_scaled_bitmap(menu_plate, 0, 0, al_get_bitmap_width(menu_plate), al_get_bitmap_height(menu_plate),plate_width_b[i], plate_height[0], al_get_bitmap_width(menu_plate)*1.2, al_get_bitmap_height(menu_plate)*1.2, 0);
        al_draw_scaled_bitmap(menu_plate_selected, 0, 0, al_get_bitmap_width(menu_plate), al_get_bitmap_height(menu_plate),plate_width_b[selected], plate_height[0], al_get_bitmap_width(menu_plate)*1.2, al_get_bitmap_height(menu_plate)*1.2, 0);
        al_draw_text(title_font,text_color, (al_get_bitmap_width(menu_plate)*3.2-10)/2, plate_height[0]+10 ,ALLEGRO_ALIGN_CENTER,"Menu");
        al_draw_text(title_font,text_color, WIDTH-al_get_bitmap_width(menu_plate)*1.6+5, plate_height[0]+10 ,ALLEGRO_ALIGN_CENTER,"Exit");

        if(floating_effect[0]>0) floating_effect[0] -= 2;
        if(floating_effect[1]>0) floating_effect[1] -= 3;

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }

    return error;
}

void game_destroy() {
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer1);
    al_destroy_timer(timer2);
    al_destroy_bitmap(image);
    al_destroy_sample(song1);
    al_destroy_sample(song2);
}
