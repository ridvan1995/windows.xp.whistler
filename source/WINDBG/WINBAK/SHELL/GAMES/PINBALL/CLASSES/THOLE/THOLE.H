//
// Copyright (c) 2000  Microsoft Corporation
//

#include "../../pinball.h"

#ifndef PINBALL_THOLE_H
#define PINBALL_THOLE_H

/* 112 */
struct THole;

void THole::TimerExpired(int, void*); // idb
int __thiscall THole::FieldEffect(THole* this, struct TBall*, struct vector_type*); // idb
int __thiscall THole::Message(THole* this, int, float); // idb
void __thiscall THole::Collision(THole* this, struct TBall*, struct vector_type*, struct vector_type*, float, struct TEdgeSegment*); // idb
THole* __thiscall THole::THole(THole* this, struct TPinballTable* a2, int a3);

void* THole::vftable = &THole::Message; // weak

#endif //PINBALL_THOLE_H
