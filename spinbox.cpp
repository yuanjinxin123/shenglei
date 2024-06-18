#include "spinbox.h"

#include <QEvent>
spinbox::spinbox(QWidget* parent) : QSpinBox(parent) {}

void spinbox::RegFilterObj(QObject* obj) {}

spinbox::~spinbox() {}

bool spinbox::getHasFouce() { return mIsHasFouce; }

void spinbox::enterEvent(QEvent* e) { mIsHasFouce = true; }

void spinbox::leaveEvent(QEvent* e) { mIsHasFouce = false; }

doubleSpinbox::doubleSpinbox(QWidget* parent) : QDoubleSpinBox(parent) {}

void doubleSpinbox::RegFilterObj(QObject* obj) {}

doubleSpinbox::~doubleSpinbox() {}

bool doubleSpinbox::getHasFouce() { return mIsHasFouce; }

void doubleSpinbox::enterEvent(QEvent* e) { mIsHasFouce = true; }

void doubleSpinbox::leaveEvent(QEvent* e) { mIsHasFouce = false; }
