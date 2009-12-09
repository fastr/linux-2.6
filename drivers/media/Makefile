#
# Makefile for the kernel multimedia device drivers.
#

media-objs	:= media-devnode.o

obj-$(CONFIG_MEDIA_SUPPORT)	+= media.o

obj-y += common/ IR/ video/

obj-$(CONFIG_VIDEO_DEV)		+= radio/
obj-$(CONFIG_DVB_CORE)		+= dvb/
