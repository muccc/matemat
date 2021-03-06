/* German Language pack ;)  */
m4_macro_rename(`ACTION', `AKTION')
m4_macro_rename(`ACTION_END', `AKTION_ENDE')
m4_macro_rename(`ACTION_DO', `AKTION_AUSFUEHREN')
m4_macro_rename(`ACTION_START', `AKTION_STARTEN')
m4_macro_rename(`ACTION_STOP', `AKTION_STOPPEN')
m4_macro_rename(`ACTION_RESTART', `AKTION_NEUSTARTEN')
m4_macro_rename(`ACTION_STARTED', `AKTION_GESTARTET')
m4_macro_rename(`CONTROL_START', `KONTROL_START')
m4_macro_rename(`CONTROL_END', `KONTROL_ENDE')
m4_macro_rename(`TIMER_NEW', `STOPUHR_NEU')
m4_macro_rename(`TIMER_START', `STOPUHR_STARTEN')
m4_macro_rename(`TIMER', `STOPUHR')
m4_macro_rename(`TIMER_WAIT', `STOPUHR_WARTEN')
m4_macro_rename(`ON', `WENN')
m4_macro_rename(`DO', `DANN')
m4_macro_rename(`END', `ENDE')
m4_macro_rename(`BETWEEN', `ZWISCHEN')
m4_macro_rename(`NOT', `NICHT')
m4_macro_rename(`PIN_INPUT', `PIN_EINGANG')
m4_macro_rename(`PIN_OUTPUT', `PIN_AUSGANG')
m4_macro_rename(`PIN_PULLUP', `PIN_HOCHZIEHEN')
m4_macro_rename(`PIN_NEW', `PIN_NEU')
m4_macro_rename(`PIN_RISING', `PIN_STEIGEND')
m4_macro_rename(`PIN_FALLING', `PIN_FALLEND')
m4_macro_rename(`CLOCK_USED', `UHR_BENUTZT')
m4_macro_rename(`CLOCK_MIN', `UHR_MINUTE')
m4_macro_rename(`CLOCK_HOUR', `UHR_STUNDE')
m4_macro_rename(`CLOCK_DAY', `UHR_TAG')
m4_macro_rename(`CLOCK_MONTH', `UHR_MONAT')
m4_macro_rename(`CLOCK_DOW', `UHR_WOCHENTAG')
m4_macro_rename(`CLOCK_YEAR', `UHR_JAHR')
m4_macro_rename(`STARTUP', `BEIM_STARTEN')

define(`PIN_AN', `PIN_SET($1)')
define(`PIN_AUS', `PIN_CLEAR($1)')
define(`PIN_UMSCHALTEN', `PIN_TOGGLE($1)')
