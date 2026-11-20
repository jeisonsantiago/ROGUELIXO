#include "npc.h"
#include "entity.h"
#include "gameplay.h"
#include "shared_context.h"

bool npc_player_in_front(Entity *self, Entity *player){

    if(!self || !player) return false;

    return ( (self->y+1 == player->y) && self->x == player->x);
}

void npc_entity_setup(Entity *entity,  char*message)
{
    NPC *npc = &entity->data.npc;

    npc->has_dialog = true;

    npc->dialog_frame_counter = 0;
    npc->current_line = 0;

    for (int i = 0; i < MAX_DIALOG_PARAGRAPHS; ++i) {
        npc->paragraph[i].start = -1;
        npc->paragraph[i].end = -1;
        npc->paragraph[i].lenght = INT_MAX;
        npc->paragraph[i].show_paragraph = false;
    }

    int message_lenght = (int)strlen(message);
    int char_index = 0;
    int dialog_w = 54;
    int dialog_box_width = dialog_w;
    int line_counter = 0;
    int paragraphs = 0;

    npc->paragraph[paragraphs].start = 0;

    while (char_index < message_lenght) {

        if(char_index > dialog_box_width){

            char c = message[char_index];
            bool find_word = false;

            if(c != ' '){
                find_word = true;
                // find the word in the (char_index--)
                while (c != ' ') {
                    // char_index-=1;
                    c = message[--char_index];
                    // append_walk = char_index;
                }
            }

            npc->dialog[char_index] = '\n';
            dialog_box_width+=dialog_w;
            line_counter++;
        }

        npc->paragraph[paragraphs].end = char_index;

        if(npc->dialog[char_index] != '\n'){
            npc->dialog[char_index] = message[char_index];
        }

        if(line_counter > 5){
            paragraphs++;
            line_counter = 0;
            npc->paragraph[paragraphs].start = (char_index+1);
        }

        char_index++;
    }

    paragraphs++;

    npc->paragraphs_count = paragraphs;

    for (int i = 0; i < npc->paragraphs_count; ++i) {
        DialogParagraph *p = &npc->paragraph[i];
        // TraceLog(LOG_INFO,"\n%s",TextSubtext(npc->dialog,p.start,p.end));
        p->show_paragraph = false;
        p->lenght = abs(p->end - p->start);
        // TraceLog(LOG_INFO,"%i %i %i",p->start,p->end,p->lenght);
    }
}

Entity *npc_at(GamePlay *gameplay, int x, int y)
{
    return NULL;
}

void npc_update(Entity *self, GamePlay *gameplay, float delta_time)
{
    NPC *npc = &self->data.npc;
    npc->dialog_frame_counter++;

    // if the typewriter animation is done, the click will iterate to the next paragraph or close
    // dialog window
    DialogParagraph *paragraph = &npc->paragraph[npc->current_paragraph];
    if(npc->is_talking && npc->dialog_frame_counter/2 > paragraph->lenght){
        paragraph->show_paragraph = true;
    }

    if(npc->is_talking && shared_context()->input_state.time_out_actions_pressed[ACTION_USE]){

        DialogParagraph *paragraph = &npc->paragraph[npc->current_paragraph];

        if(!paragraph->show_paragraph){
            paragraph->show_paragraph = true;
        }else{
            npc->current_paragraph++;
        }
    }

    if(npc_player_in_front(self,&gameplay->player) ){
        if(shared_context()->input_state.time_out_actions_pressed[ACTION_USE]){
            npc->is_talking = true;
            npc->dialog_frame_counter = 0;
        }
    }else{
        npc->is_talking = false;
        npc->current_paragraph = 0;

        for (int i = 0; i < npc->paragraphs_count; ++i) {
            npc->paragraph[i].show_paragraph = false;
        }
    }

    if(npc->current_paragraph == npc->paragraphs_count){
        npc->is_talking = false;
        npc->current_paragraph = 0;

        for (int i = 0; i < npc->paragraphs_count; ++i) {
            npc->paragraph[i].show_paragraph = false;
        }
    }
}
