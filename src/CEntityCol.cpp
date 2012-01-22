#include "CEntity.h"

std::map<std::pair<int, int>, CEntityCol> CEntityCol::EntityColList;

CEntityCol::CEntityCol() {
    this->EntityA = NULL;
    this->EntityB = NULL;
}

void CEntityCol::updateContactSize() {

  int oX = EntityA->NextX + EntityA->Col_X;
  int oY = EntityA->NextY + EntityA->Col_Y;

  int oW = EntityA->Width - EntityA->Col_Width;
  int oH = EntityA->Height - EntityA->Col_Height;

  int left1, left2;
  int right1, right2;
  int top1, top2;
  int bottom1, bottom2;

  int tX = (int)EntityB->NextX + EntityB->Col_X;
  int tY = (int)EntityB->NextY + EntityB->Col_Y;

  left1 = tX;
  left2 = oX;

  right1 = left1 + EntityB->Width - 1 - EntityB->Col_Width;
  right2 = oX + oW - 1;

  top1 = tY;
  top2 = oY;

  bottom1 = top1 + EntityB->Height - 1 - EntityB->Col_Height;
  bottom2 = oY + oH - 1;

  Height = 0; // for now height and width are always >=0
  Width = 0;

  if (bottom1 < top2) return;
  if (top1 > bottom2) return;

  if (right1 < left2) return;
  if (left1 > right2) return;

  Width = std::min( abs(right1 - left2), abs(right2 - left1));
  Height = std::min( abs(bottom1 - top2), abs(bottom2 - top1));

}
