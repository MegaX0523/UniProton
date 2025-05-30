set(SRC main.c ctltask.c filter.c gpio_set.c spi.c)
add_library(rpmsg OBJECT ${SRC})
