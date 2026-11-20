#ifndef NPC_H
#define NPC_H

typedef struct Entity Entity;
typedef struct Map Map;
typedef struct GamePlay GamePlay;

#define MAX_DIALOG_PARAGRAPHS 20

typedef struct DialogParagraph{
    int start;
    int end;
    int lenght;
    bool show_paragraph;
}DialogParagraph;

typedef struct NPC{
    bool has_dialog;
    bool is_talking;

    char dialog[STR_LENGHT_MESSAGE];

    int current_line;
    DialogParagraph paragraph[MAX_DIALOG_PARAGRAPHS];
    int paragraphs_count;
    int current_paragraph;

    bool is_player_in_front;
    int dialog_frame_counter;

    // int ui_render_current_line;
}NPC;

void npc_entity_setup(Entity *entity,  char*message);
Entity *npc_at(GamePlay *gameplay, int x, int y);
void npc_update(Entity *self, GamePlay *gameplay, float delta_time);
bool npc_player_in_front(Entity *self, Entity *player);

#endif // NPC_H
