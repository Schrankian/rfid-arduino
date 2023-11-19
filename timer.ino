// The Hardware part for the project
// Starts the timer register
// Use like this:
/*
startTimer(3);
*/

bool timerIsActive = false;

void startTimer(unsigned int seconds) {
  if (seconds > 4 || seconds < 1 || timerIsActive) return;

  // Stop all interupts
  cli();

  timerIsActive = true;

  // TODO why?
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Initializer counter value to 0;
  TCNT1 = 0;
  // Set timer count for [seconds]-second increments
  OCR1A = 16000000 / (1024 * (1.0 / seconds)) - 1;
  // Enabler timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Allow interrupts
  sei();
}

void stopTimer() {
  if (!timerIsActive) return;
  // Stop all interrupts
  cli();

  // Clear the Prescaler bits
  TCCR1B = 0;
  timerIsActive = false;

  // Allow interrupts
  sei();
}

ISR(TIMER1_COMPA_vect) {
  onCoolDown = false;
  cooldownFinished = true;
  Serial.println("Interrupt called");

  stopTimer();  // If not stoped, it loops
}