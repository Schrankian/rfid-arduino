// The Hardware part for the project
// Starts the timer register
// Use like this:
/*
startTimer(3); // -> time in whole seconds (int)
*/

void startTimer(unsigned int seconds) {
  if (seconds > 4 || seconds < 1) return;

  // Stop all interupts
  cli();

  // Initializer counter value to 0;
  TCNT1 = 0;
  // Clean all lingering interrupt flags, which could potentially lead to an interrupt during the first configuration
  TIFR1 |= (1 << OCF1A);
  // Reset the timer configuration registers
  TCCR1A = 0;
  TCCR1B = 0;
  // Set to CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for prescaler 1024, so it counts slower
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Set timer count for [seconds]-second increments
  OCR1A = 16000000 / (1024 * (1.0 / seconds)) - 1;
  // Enabler timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Allow interrupts
  sei();
}

void stopTimer() {
  // Stop all interrupts
  cli();

  // Clear the Prescaler bits
  TCCR1B = 0;

  // Allow interrupts
  sei();
}

// Catch interrupt
ISR(TIMER1_COMPA_vect) {
  // Notify the display, that the timer has finished
  cooldownFinished = true;

  // Stop the timer again (one shot). If not, it starts again
  stopTimer();
}