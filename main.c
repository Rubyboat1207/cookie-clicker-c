#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<stdlib.h>
#include"utils.h"
#include"factory.h"
#include"grandma.h"
#include"monkey.h"
#include"tree.h"
#include"events.h"
#include"raylib.h"

double cookies = 0;

void shop();

bool strEq(char* a, char* b) {
    return strcmp(a, b) == 0;
}

void clean_input_buffer(char* data) {
    size_t len = strlen(data);
    if (len > 0 && data[len - 1] == '\n') {
        data[len - 1] = '\0';
    } else {
        // If there's no newline, clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

#define FACTORY_TYPE_COUNT 3
FactoryType* factory_types[FACTORY_TYPE_COUNT];

void init_factory_types() {
    factory_types[0] = init_grandma();
    factory_types[1] = init_monkey();
    factory_types[2] = init_tree();
}

int event_countdown = 0;
RandomEventType* active_event = NULL;

double cookie_size = 2;

typedef struct ClickGraphicPosition {
    int life_frames;
    Vector2 pos;
} ClickGraphicPosition;

ClickGraphicPosition* click_graphics[256];

void on_click() {
    ++cookies;
    cookie_size += 0.3;
    for(char i = 0; i < 256; i++) {
        if(click_graphics[i] == NULL) {
            click_graphics[i] = calloc(1, sizeof(ClickGraphicPosition));
            click_graphics[i]->life_frames = 60;
            click_graphics[i]->pos = GetMousePosition();
            click_graphics[i]->pos.x -= rand_range(-10, 10);
            click_graphics[i]->pos.y -= rand_range(-10, 10);
            break;
        }
    }
    // if(rand_range(0, 9000) == 5 && active_event == NULL) {
    //     active_event = get_random_event();
    //     active_event->on_triggered();
    //     event_countdown = 50;
    // }
    // if(active_event != NULL) {
    //     if(active_event->on_clicked(&cookies)) {
    //         active_event = NULL;
    //     }

    //     if(event_countdown == 0) {
    //         active_event->on_timeout();
    //         active_event = NULL;
    //     }
    //     event_countdown -= 1;
    // }

}

void calculate_factories(long long ms_diff) {
    for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
        FactoryType* type = factory_types[i];

        double count = type->instances->length * (ms_diff / 1000.0) * type->cookies_per_second;

        if(count > 1) {
            printf("Your %ss have produced %d cookies.\n", type->name, (int) round(count));
        }

        cookies += count;
    }
}

EventTimeBuffer* click_buffer;

Texture2D cookie;
#define FRAMERATE (1.0/60.0)

#define DATA_SIZE 5
int main() {
    srand(time_ms());

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Cookie Clicker");

    cookie = LoadTexture("./assets/cookie.png");
    init_factory_types();
    SetTargetFPS(60);

    click_buffer = new_rolling_time_buffer(64, 1000);

    Vector2 center = {screenWidth / 2, screenHeight / 2};
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        char* cookie_text = get_large_number_text(round(cookies));

        int cookie_text_width = MeasureText(cookie_text, 48);
        DrawText(cookie_text, screenWidth / 2 - cookie_text_width / 2, 50, 48, BLACK);

        Vector2 cookiePos = {screenWidth / 2 - cookie.width / 2 * cookie_size, screenHeight / 2 - cookie.height / 2 * cookie_size};
        DrawTextureEx(cookie, cookiePos, 0, cookie_size, WHITE);

        Vector2 mousePos = GetMousePosition();

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && sqrt(pow(center.x - mousePos.x, 2) + pow(center.y - mousePos.y, 2)) < cookie.width) {
            on_click();
            etb_add(click_buffer);
        }

        if(cookie_size > 2) {
            cookie_size -= 1 * FRAMERATE * cookie_size;
        }else {
            cookie_size = 2;
        }

        for(int i = 0; i < 256; i++) {
            ClickGraphicPosition* pos = click_graphics[i];
            if(pos != NULL) {
                if(pos->life_frames == 0) {
                    free(pos);
                    click_graphics[i] = NULL;
                }else {
                    pos->life_frames -= 1;
                    Color c = {0, 0, 0, pos->life_frames * 255 / 60};
                    DrawText("+1", pos->pos.x, pos->pos.y - (60 - pos->life_frames), 24, c);
                }
            }
        }

        double cookies_per_second = 0;

        for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
            FactoryType* type = factory_types[i];
            Vector2 pos = {screenWidth - type->texture->width, i * type->texture->height};
            DrawTexture(*type->texture, pos.x, pos.y, WHITE);
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && inside_box(GetMousePosition(), pos, (Vector2){type->texture->width, type->texture->height})) {
                int cost = type->get_cost(type);
                if(cost > cookies) {
                    printf("You dont have enough money.\n");
                    continue;
                }
                cookies -= cost;
                type->create_instance(type);
            }
            //draw count
            char* count = calloc(1, 20);
            sprintf(count, "%d", type->instances->length);
            int count_width = MeasureText(count, 24);
            DrawText(count, pos.x - 10 - count_width, pos.y, 24, BLACK);
            free(count);

            //draw cost
            char* cost = type->get_print_cost(type);
            char* cost_text = calloc(1, 20);
            sprintf(cost_text, "$%s", cost);
            int cost_width = MeasureText(cost_text, 24);
            DrawText(cost_text, pos.x - 10 - cost_width, pos.y + 30, 24, BLACK);
            free(cost);
            free(cost_text);

            cookies_per_second += type->instances->length * type->cookies_per_second;
        }

        cookies_per_second += etb_events_count(click_buffer);

        //draw cookies per second
        char* cps_text = calloc(1, 128);
        sprintf(cps_text, "%.2f cookies per second", cookies_per_second);
        int cps_width = MeasureText(cps_text, 24);
        DrawText(cps_text, screenWidth - cps_width - 10, screenHeight - 30, 24, BLACK);
        free(cps_text);
        

        calculate_factories(1000 / 60);

        EndDrawing();
    }

    for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
        factory_types[i]->free(factory_types[i]);
    }

    CloseWindow();
    printf("all resources freed");

    return 0;
}

void shop() {
    while(1) {
        printf("---- SHOP ----\n\nCOOKIES: %d\n\n", (int) round(cookies));
        printf("0 - Exit\n");
        for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
            char* cost = factory_types[i]->get_print_cost(factory_types[i]);
            printf("%d - %s $%s (you own: %d)\n", i + 1, factory_types[i]->name, cost, factory_types[i]->instances->length);
            free(cost);
        }

        char val[10];
        fgets(val, 10, stdin);
        clean_input_buffer(val);

        if(strEq(val, "0")) {
            return;
        }

        int num = atoi(val);
    }
    
}