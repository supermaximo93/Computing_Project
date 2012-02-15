/*
 * PartController.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef PARTCONTROLLER_H
#define PARTCONTROLLER_H

#include "Database.h"

class QWidget;
class Part;

namespace PartController
{

void Index(QWidget * caller);

void Show(int partId, QWidget * caller);
void Show(Part & part, QWidget * caller);

Part New(QWidget * caller);

void Edit(int partId, QWidget * caller);
void Edit(Part & part, QWidget * caller);

bool Create(Part & partAttributes, QWidget * caller);

bool Update(const Part & part, QWidget * caller);

bool Destroy(int partId, QWidget * caller);
bool Destroy(Part & part, QWidget * caller);

Part getPart(int partId);
Database<Part>::recordListPtr getAllParts();

}

#endif // PARTCONTROLLER_H
