#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/input.h>
#include <unistd.h>

void print_usage(FILE *stream, const char *program) {
  fprintf(stream,
          "w2w - Transform a tablet wheel (or ring) in a mouse wheel\n"
          "\n"
          "%s [-h | [-s steps]]\n"
          "Mainly to use with the ring of my HUION\n"
          "options:\n"
          "   -h           show this message and exit\n",
          program);
}

bool read_event(struct input_event *event) {
  return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
  if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
    exit(EXIT_FAILURE);
}

void write_scroll(const int steps) {
  const struct input_event low_res_event = {.type = EV_REL,
                                            .code = REL_WHEEL,
                                            .value = steps},
                           high_res_event = {.type = EV_REL,
                                             .code = REL_WHEEL_HI_RES,
                                             .value = steps * 120};
  write_event(&low_res_event);
  write_event(&high_res_event);
}
const int RING_SIZE = 12;
int main(int argc, char *argv[]) {
  for (int opt; (opt = getopt(argc, argv, "h:")) != -1;) {
    switch (opt) {
    case 'h':
      return print_usage(stdout, argv[0]), EXIT_SUCCESS;
    }

    return print_usage(stderr, argv[0]), EXIT_FAILURE;
  }

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  struct input_event input;
  bool scrolling = false;
  int last_position = 0;

  int low_res;
  while (read_event(&input)) {
    // Unhandled event, just bypassit it
    if (input.type != EV_ABS) {
      write_event(&input);
      continue;
    }

    if (input.code == ABS_MISC && input.value) {
      last_position = 0;
      scrolling = false;
    }
    if (input.code == ABS_WHEEL) {
      if (scrolling) {
        low_res = (input.value - last_position);
        last_position = input.value;
        int sign = (low_res > 0) - (low_res < 0);
        low_res = abs(low_res) * 2 < RING_SIZE
                      ? low_res
                      : -sign * (RING_SIZE - abs(low_res));

        if (low_res) {
          write_scroll(low_res);
        }
      } else {
        last_position = input.value;
        scrolling = true;
      }
    }
  }
}
