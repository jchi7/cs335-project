#ifndef MARKS_H
#define MARKS_H

bool currentlyEditable(Game *);
void editorAddPlatform(Game *game, GameObject * mouse);
void editorAddSpike(Game *game, GameObject * mouse);
void editorAddSavePoint(Game *game, GameObject * mouse);
void editorAddElevator(Game *game, GameObject * mouse);
void editorAddEnemy(Game *game, GameObject * mouse);
void resizePlatform(Game *game, GameObject * mouse);
void resizeElevator(Game *game, GameObject * mouse);
void editorRemovePlatform(Game *game, int index);
void editorRemoveSpike(Game *game, int index);
void editorRemoveSavePoint(Game *game, int index);
void editorRemoveElevator(Game *game, int index);
void editorRemoveEnemy(Game *game, int index);
void nextEnemy(Game *, GameObject *);
void movablePlatformCollision(GameObject *, GameObject *);

#endif // MARKS_H
