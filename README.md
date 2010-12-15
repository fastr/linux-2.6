FSR172X Kernel Support
====

Obtain the choice kernels

    git clone git://github.com/mirrors/linux-2.6.git
    git remote remane origin torvalds
    git remote add fastr git://github.com/fastr/linux-2.6.git
    git remote add sakoman git://www.sakoman.com/git/linux-omap-2.6
    git remote -v

Checkout local copies of our favorite branches / tags

    git fetch torvalds
    git fetch torvalds --tags
    git checkout -b torvalds-2.6.36 tags/v2.6.36

    git fetch sakoman
    git checkout -b sakoman-2.6.36 sakoman/omap-2.6.36

    git fetch fastr
    git checkout -b fsr172x-2.6.36 fastr/fsr172x-2.6.36

Resolve the best of all worlds

    git checkout torvalds-2.6.35
    git branch overo-fsr172x-2.6.36

    git merge fsr172x-2.6.36
    git merge sakoman-2.6.36 # git thinks there is a conflict, but it's just an addition in the same place
    git mergetool # just delete the HEAD<<<<, ====, xyz>>>>>> stuff, it's just an addition

    git add arch/arm/mach-omap2/board-overo.c
    git commit -m "added fsr172x general data acquisition support to gumstix overo"
    git push origin overo-fsr172x-2.6.36

    git remote set-head fastr overo-fsr172x-2.6

Backdrop
====

    mkdir fsr172x.patches
    tar xf omap3isp-fsr172x-patches.tar.gz -C fsr172x.patches
    cd linux-2.6
    git checkout torvalds-2.6.36
    git branch fsr172x-2.6.36
    git checkout fsr172x-2.6.36
    git am ../fsr172x.patches/*

    Applying: v4l: Share code between video_usercopy and video_ioctl2
    Applying: v4l: subdev: Don't require core operations
    Applying: v4l: subdev: Merge v4l2_i2c_new_subdev_cfg and v4l2_i2c_new_subdev
    Applying: v4l: subdev: Add device node support
    Applying: v4l: subdev: Uninline the v4l2_subdev_init function
    Applying: v4l: subdev: Control ioctls support
    Applying: v4l: subdev: Events support
    Applying: media: Media device node support
    Applying: media: Media device
    /home/fastr/linux-2.6/.git/rebase-apply/patch:80: new blank line at EOF.
    +
    warning: 1 line adds whitespace errors.
    Applying: media: Entities, pads and links
    /home/fastr/linux-2.6/.git/rebase-apply/patch:76: new blank line at EOF.
    +
    warning: 1 line adds whitespace errors.
    Applying: media: Entity graph traversal
    /home/fastr/linux-2.6/.git/rebase-apply/patch:11: new blank line at EOF.
    +
    warning: 1 line adds whitespace errors.
    Applying: media: Reference count and power handling
    /home/fastr/linux-2.6/.git/rebase-apply/patch:13: new blank line at EOF.
    +
    warning: 1 line adds whitespace errors.
    Applying: media: Media device information query
    Applying: media: Entities, pads and links enumeration
    Applying: media: Links setup
    Applying: media: Entity locking and pipeline management
    Applying: v4l: Add a media_device pointer to the v4l2_device structure
    Applying: v4l: Make video_device inherit from media_entity
    Applying: v4l: Make v4l2_subdev inherit from media_entity
    Applying: v4l: Move the media/v4l2-mediabus.h header to include/linux
    Applying: v4l: Replace enums with fixed-sized fields in public structure
    Applying: v4l: Rename V4L2_MBUS_FMT_GREY8_1X8 to V4L2_MBUS_FMT_Y8_1X8
    Applying: v4l: Group media bus pixel codes by types and sort them alphabetically
    Applying: v4l: Add 8-bit YUYV on 16-bit bus and SGRBG10 media bus pixel codes
    Applying: v4l: Add remaining RAW10 patterns w DPCM pixel code variants
    Applying: v4l: Create v4l2 subdev file handle structure
    Applying: v4l: v4l2_subdev pad-level operations
    Applying: v4l: v4l2_subdev userspace format API - documentation binary files
    Applying: v4l: v4l2_subdev userspace format API
    Applying: v4l: v4l2_subdev userspace frame interval API
    Applying: v4l: v4l2_subdev userspace crop API
    Applying: v4l: subdev: Generic ioctl support
    Applying: v4l: Load I2C modules based on modalias
    Applying: v4l: Remove module_name argument to the v4l2_i2c_new_subdev* functions
    Applying: V4L/DVB: v4l: Use v4l2_get_subdevdata instead of accessing v4l2_subdev::priv
    Applying: V4L/DVB: v4l: Add a v4l2_subdev host private data field
    Applying: ARM: OMAP3: Update Camera ISP definitions for OMAP3630
    Applying: omap3: Export omap3isp platform device structure
    Applying: OMAP3 ISP driver
    Applying: omap3isp: Rebase on top of 2.6.36-rc5
    Applying: omap3isp: Update to the latest media controller API
    Applying: v4l: Add missing 12 bits bayer media bus formats
    Applying: v4l: Add 12 bits bayer pixel formats
    Applying: omap3isp: Add 12 bit bayer support at the CCDC input

    git add README.md
    git commit -m "added fsr172x general data acquisition support to omap"
    git push origin fsr172x-2.6.36
