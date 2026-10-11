# BASSFORGE v0.9 — Precision Engine (experimental)

Preserves the v0.8 interface and adds a matching **PRECISION ENGINE** toggle on the SYNTH page. Off by default for backwards compatibility. When on: reduced drive transfer intensity, less upper-mid emphasis, lower filter ceiling, gentler transient boost and extra output headroom.

**Not included:** true oversampling, multiband dynamics, spectral resonance detection, audio-verified mastering. These require further DSP work and tests. No claim of release-ready sound without auditioning the rendered output.

Preset pack is in the separate `BASSFORGE-Precision-Neuro-Vol-1.zip`. The six presets enable Precision Engine and default to FX disabled. Preset values are normalized (0 to 1).

Build with GitHub Actions in `.github/workflows/build-windows.yml`. JUCE Windows compilation and FL Studio testing have NOT been performed here. Keep your working v0.8 VST3 backup.
