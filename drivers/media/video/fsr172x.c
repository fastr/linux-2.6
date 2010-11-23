/*
 * Driver for FSR172x data acquisition system
 *
 * Copyright (C) 2010, Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/i2c.h>
#include <linux/log2.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <linux/v4l2-mediabus.h>

#include <mach/gpio.h>

#include <media/fsr172x.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>

#define FSR172X_WIDTH		2048
#define FSR172X_HEIGHT		128

struct fsr172x {
	struct v4l2_subdev subdev;
	struct media_pad pad;
	unsigned int gpio;
};

static struct fsr172x *to_fsr172x(struct v4l2_subdev *sd)
{
	return container_of(sd, struct fsr172x, subdev);
}

/* -----------------------------------------------------------------------------
 * V4L2 subdev operations
 */

static int fsr172x_s_stream(struct v4l2_subdev *subdev, int enable)
{
	struct fsr172x *fsr172x = to_fsr172x(subdev);

	gpio_set_value(fsr172x->gpio, enable ? 1 : 0);

	return 0;
}

static int fsr172x_enum_mbus_code(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->index > 0)
		return -EINVAL;

	code->code = V4L2_MBUS_FMT_SGRBG12_1X12;
	return 0;
}

static int fsr172x_enum_frame_size(struct v4l2_subdev *subdev,
				   struct v4l2_subdev_fh *fh,
				   struct v4l2_subdev_frame_size_enum *fse)
{
	if (fse->index > 0 || fse->pad != 0)
		return -EINVAL;

	fse->code = V4L2_MBUS_FMT_SGRBG12_1X12;
	fse->min_width = FSR172X_WIDTH;
	fse->max_width = FSR172X_WIDTH;
	fse->min_height = FSR172X_HEIGHT;
	fse->max_height = FSR172X_HEIGHT;

	return 0;
}

static int fsr172x_get_format(struct v4l2_subdev *subdev,
			      struct v4l2_subdev_fh *fh,
			      struct v4l2_subdev_format *format)
{
	format->format.code = V4L2_MBUS_FMT_SGRBG12_1X12;
	format->format.width = FSR172X_WIDTH;
	format->format.height = FSR172X_HEIGHT;
	format->format.field = V4L2_FIELD_NONE;
	format->format.colorspace = V4L2_COLORSPACE_SRGB;

	return 0;
}

static int fsr172x_set_format(struct v4l2_subdev *subdev,
			      struct v4l2_subdev_fh *fh,
			      struct v4l2_subdev_format *format)
{
	format->format.code = V4L2_MBUS_FMT_SGRBG12_1X12;
	format->format.width = FSR172X_WIDTH;
	format->format.height = FSR172X_HEIGHT;
	format->format.field = V4L2_FIELD_NONE;
	format->format.colorspace = V4L2_COLORSPACE_SRGB;

	return 0;
}

static struct v4l2_subdev_video_ops fsr172x_subdev_video_ops = {
	.s_stream	= fsr172x_s_stream,
};

static struct v4l2_subdev_pad_ops fsr172x_subdev_pad_ops = {
	.enum_mbus_code = fsr172x_enum_mbus_code,
	.enum_frame_size = fsr172x_enum_frame_size,
	.get_fmt = fsr172x_get_format,
	.set_fmt = fsr172x_set_format,
};

static struct v4l2_subdev_ops fsr172x_subdev_ops = {
	.video	= &fsr172x_subdev_video_ops,
	.pad	= &fsr172x_subdev_pad_ops,
};

/* -----------------------------------------------------------------------------
 * Driver registration and device probing
 */

static int fsr172x_probe(struct i2c_client *client,
			 const struct i2c_device_id *did)
{
	struct fsr172x_platform_data *pdata = client->dev.platform_data;
	struct fsr172x *fsr172x;
	int ret;

	dev_info(&client->dev, "FSR172x detected\n");

	if (pdata == NULL)
		return -EINVAL;

	fsr172x = kzalloc(sizeof(*fsr172x), GFP_KERNEL);
	if (!fsr172x)
		return -ENOMEM;

	fsr172x->gpio = pdata->gpio;

	v4l2_i2c_subdev_init(&fsr172x->subdev, client, &fsr172x_subdev_ops);
	fsr172x->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;

	fsr172x->pad.flags = MEDIA_PAD_FLAG_OUTPUT;
	ret = media_entity_init(&fsr172x->subdev.entity, 1, &fsr172x->pad, 0);
	if (ret < 0) {
		kfree(fsr172x);
		return ret;
	}

	if (gpio_request(fsr172x->gpio, "FSR172X_GPIO_STREAM") < 0 ||
	    gpio_direction_output(fsr172x->gpio, 0) < 0) {
		dev_info(&client->dev, "Unable to configure GPIO");
		kfree(fsr172x);
		return -ENODEV;
	}

	return 0;
}

static int fsr172x_remove(struct i2c_client *client)
{
	struct v4l2_subdev *subdev = i2c_get_clientdata(client);
	struct fsr172x *fsr172x = to_fsr172x(subdev);

	gpio_free(fsr172x->gpio);

	v4l2_device_unregister_subdev(subdev);
	media_entity_cleanup(&subdev->entity);
	kfree(fsr172x);
	return 0;
}

static const struct i2c_device_id fsr172x_id[] = {
	{ "fsr172x", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, fsr172x_id);

static struct i2c_driver fsr172x_driver = {
	.driver = {
		.name = "fsr172x",
	},
	.probe		= fsr172x_probe,
	.remove		= fsr172x_remove,
	.id_table	= fsr172x_id,
};

static int __init fsr172x_init(void)
{
	return i2c_add_driver(&fsr172x_driver);
}

static void __exit fsr172x_exit(void)
{
	i2c_del_driver(&fsr172x_driver);
}

module_init(fsr172x_init);
module_exit(fsr172x_exit);

MODULE_DESCRIPTION("FSR172x ADC driver");
MODULE_AUTHOR("Laurent Pinchart <laurent.pinchart@ideasonboard.com>");
MODULE_LICENSE("GPL");
