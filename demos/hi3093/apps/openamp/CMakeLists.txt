set(SRC main.c gpio_set.c spi.c AD7606.c DAC8563.c ctltask.c filter.c sample_gpio.c sample_common.c)
add_library(rpmsg OBJECT ${SRC})
