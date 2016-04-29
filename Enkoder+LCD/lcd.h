void lcd_init();

void lcd_putchar(char c);

void lcd_puttext_P(const char __flash *txt);

void lcd_puttext(const char __memx *txt);

void lcd_goto(uint8_t x, uint8_t y);

void lcd_cls();

void lcd_defchar(uint8_t charno, const uint8_t __memx *chardef);

void lcd_box(uint8_t y);