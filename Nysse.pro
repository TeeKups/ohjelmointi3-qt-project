TEMPLATE = subdirs

SUBDIRS += \
	CourseLib \
	Game

CourseLib.subdir = Course/CourseLib
Game.depends = CourseLib
