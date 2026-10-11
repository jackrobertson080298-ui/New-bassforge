# BASSFORGE v0.7 — LFO Edition

Built from the existing v0.5 Complete UI Redesign source. The original SYNTH and FX views, 30 factory patches, and .bfpreset format remain in place. The third LFO page matches the dark navy / amber visual language.

Three per-voice LFOs have six shapes (Sine, Triangle, Saw, Square, Ramp, Custom), six synced rates (1/16 to 2 bars), retrigger / free-run / one-shot modes, six destinations (FM, Cutoff, Drive, Rasp, Movement, Pitch), and bipolar modulation depth. Default depth is 0 so old sounds do not change until modulation is enabled. BPM is read from the host transport, with a 174 BPM fallback for standalone operation. The LFO waveform previews are UI representations, not audio scopes.

## GitHub build
Upload the CONTENTS of the BASSFORGE directory to the repository root, with CMakeLists.txt alongside Source and .github. The workflow is at .github/workflows/build-windows.yml. The previous preset lab is also included separately in the ZIP.

## Important testing limitations
Not compiled on Windows or listened to in FL Studio in this environment. Please test: build, LFO rates under host BPM changes, one-shot behaviour, save/load state, imported older .bfpreset patches, keyboard, and old FM Punisher / Final Boss sound at zero LFO depth. No new factory preset bank is claimed to be auditioned.

## Known design tradeoffs
Modulation depth is unipolar 0..100% but waveform modulation is bipolar around the existing parameter. Modulation is applied inside the voice signal path. Drive / rasp modulation can produce high-frequency content; use lower depths and filter cutoff when designing presets. Modulation is per-voice and the free-run mode uses plugin time, not DAW bar-position phase locking.
