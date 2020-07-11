// VM class

#include <string>
#include <queue>
#include <stdint.h>
#include <interpreter.h>
#include <fatfilesystem.h>

#include <circle/memory.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <SDCard/emmc.h>
#include <fatfs/ff.h>
#include <circle/types.h>

typedef struct {
  uint32_t type;
  uint32_t timestamp;
  uint32_t windowID;
  uint32_t which;
  uint32_t state;
  int32_t x;
  int32_t y;
  int32_t xrel;
  int32_t yrel;
} MouseMotionEvent;

typedef struct {
  uint32_t type;
  uint32_t timestamp;
  uint32_t windowID;
  uint32_t which;
  uint8_t  button;
  uint8_t  state;
  int32_t  x;
  int32_t  y;
} MouseButtonEvent;
 
typedef struct {
  uint32_t scancode;
  uint32_t sym;
  uint16_t mod;
  uint32_t unused;
} KeySym;

typedef struct {
  uint32_t type;
  uint32_t timestamp;
  uint32_t windowID;
  uint8_t  state;
  uint8_t  repeat;
  KeySym  keysym;
} KeyboardEvent;
 
typedef struct { int x; int y; int w; int h; } Rect;


struct options
{
    std::string root_directory;
    std::string snapshot_name;
    bool        three_buttons;
    int         cycles_per_frame;
    int         display_scale;
    bool        vsync;
    uint32_t    novsync_delay;
};

class VirtualMachine: public IHardwareAbstractionLayer
{
public:
    VirtualMachine(struct options& vm_options, CScreenDevice& m_Screen) :
        vm_options(vm_options),
        fileSystem(vm_options.root_directory),
        interpreter(this, &fileSystem),
        display_width(0), display_height(0),
        scheduled_semaphore(0), input_semaphore(0), scheduled_time(0),
        event_count(0), last_event_time(0),
        quit_signalled(false), texture_needs_update(false),
        image_name(vm_options.snapshot_name),
        m_Screen(m_Screen), ticks(0),
	old_mouseX(0), old_mouseY(0)
    {
    }
    
    ~VirtualMachine()
    {
    }

    void set_input_semaphore(int semaphore);
    std::uint32_t get_smalltalk_epoch_time(void);
    std::uint32_t get_msclock(void);
    void signal_at(int semaphore, std::uint32_t msClockTime);
    void set_cursor_image(std::uint16_t *image);
    void set_cursor_location(int x, int y);
    void get_cursor_location(int *x, int *y);
    bool set_display_size(int width, int height);
    void display_changed(int x, int y, int width, int height);
    bool next_input_word(std::uint16_t *word);
    void error(const char *message);
    void signal_quit();
    void set_link_cursor(bool link);
    void exit_to_debugger(void);
    void check_scheduled_semaphore();
    void update_mouse_cursor(const std::uint16_t* cursor_bits);
    void queue_input_word(uint16_t);
    void set_image_name(const char *new_name);
    void initialize_texture(void);
    void update_texture(void);
    void update_cursor(int, int);
    void process_events(void);
    void render(void);
    const char *get_image_name(void);
    bool init(void);
    void run(void);
    void queue_input_word(uint16_t, uint16_t);
    void queue_input_time_words();
    void handle_keyboard_event(int);
    void handle_mouse_button_event(unsigned, int);
    void handle_mouse_movement_event(int, int);

    struct options vm_options;

    FatST80FileSystem fileSystem;

    int screen_initialized;

private:
    Interpreter interpreter;

    std::queue<std::uint16_t> input_queue;
    std::uint32_t last_event_time;
    int event_count;

    int input_semaphore;
    bool quit_signalled;

    bool texture_needs_update;
    int display_width, display_height;

    Rect dirty_rect;

    int scheduled_semaphore;
    std::uint32_t scheduled_time;
    std::string image_name;

    CScreenDevice& m_Screen;

    int off_x = 0;
    int off_y = 0;

    std::uint32_t ticks;
    std::uint16_t MyCursorSymbol[16] = {0};
    std::uint16_t MyMouseBackground[32];
    int old_mouseX, old_mouseY;
};
