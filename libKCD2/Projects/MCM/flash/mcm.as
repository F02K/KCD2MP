// MCM menu -- frame 1 DoAction (AS2 / Flash 8 / Scaleform GFx).
//
// ARCHITECTURE (v3): the plugin pushes EVERY mod's settings once per open
// (granular typed calls, no packed strings); flash caches them in modCache and
// owns mod switching, panel building and per-setting resets locally. The only
// outbound traffic is single user-paced events.
//
// Engine API (inbound, on _root -- see MCM.xml):
//   fc_beginMods()
//   fc_addMod(modId, uiName)
//   fc_addCategory(modId, label, tip)
//   fc_addToggle(modId, id, label, tip, value, def)
//   fc_addDropdown(modId, id, label, tip, "opt|opt|...", index, defIndex)
//   fc_addSlider(modId, id, label, tip, min, max, step, value, def, suffix)
//   fc_showMods()                 -- build list, show first mod
// Events (outbound via ExternalInterface.call):
//   onValueChanged(modIdx, itemIdx, value), onBack()
// Keybinds arrive NATIVELY: the plugin's IInputEventListener forwards X/Esc
// via fc_setInput (the engine only delivers Esc to the movie as a GFx key
// event; search typing rides char events). Flash owns the search-focus
// semantics: Esc while typing leaves the search box, otherwise onBack.
//
// Layout constants are ABSOLUTE pixels of menu.png (1573x1000), drawn inside
// the "ui" clip scaled to WIN_SCALE% of the 800x450 stage; hint constants are
// stage pixels. All values are final (tuned via screenshot iterations).
// NOTE: written for FFDec's AS2 parser -- one declaration per var, no object
// literals, no ternary, no chained assignments.

// ------------------------------------------------------- layout constants --
var WIN_X = 50;
var WIN_Y = 2;
var WIN_SCALE = 44.5;
var TITLE_X = 210;
var TITLE_Y = 112;
var TITLE_W = 380;
var MOD_X = 118;
var MOD_Y = 185;
var MODROW_W = 420;
var MODROW_H = 66;
var MRAIL_X = 548;
var MRAIL_Y = 185;
var MRAIL_W = 44;
var MRAIL_H = 700;
var HEAD_X = 640;
var HEAD_Y = 104;
var HEAD_W = 780;
var SEP_X = 640;
var SEP_Y = 168;
var SEP_W = 780;
var SEP_H = 60;
var SEARCH_X = 905;
var SEARCH_Y = 236;
var SEARCH_W = 515;
var SEARCH_H = 52;
var VIEW_X = 640;
var VIEW_Y = 306;
var VIEW_W = 780;
var VIEW_H = 590;
var SRAIL_X = 1426;
var SRAIL_Y = 256;
var SRAIL_W = 44;
var SRAIL_H = 647;
var HINT_Y = 431;
var HINT_X0 = 20;
var HINT_X1 = 150;
var HINT_X3 = 716;

// row internals (menu.png pixels). COL_SPLIT is the invisible center line of
// the view: labels live left of it, controls right of it; whatever would
// cross wraps onto another line instead. Tips show as hover tooltips.
var ROW_H = 72;
var ROW_H_SLIDER = 96;
var CAT_H = 64;
var SLIDER_W = 340;
var COL_SPLIT = VIEW_W / 2;
var SIZE_LABEL = 28;
var SIZE_VALUE = 21;
var SIZE_TIP = 17;
var SIZE_CAT = 37;
var CTRL_R = VIEW_W - 20;

// control metrics (formerly inline magic numbers; values unchanged)
var KNOB_W = 44;
var KNOB_H = 38;
var KNOB_HALF = 22;
var MODKNOB_MIN = MRAIL_Y + 50;
var MODKNOB_MAX = MRAIL_Y + MRAIL_H - 90;
var CONTENTKNOB_MIN = SRAIL_Y + 57;
var CONTENTKNOB_MAX = SRAIL_Y + SRAIL_H - 90;
var WHEEL_STEP = 54;
var SLIDER_INSET_L = 18;
var SLIDER_INSET_R = 62;
var TOGGLE_X_INSET = 56;
var ARROW_X_INSET = 54;
var MODLABEL_X = 80;
var MODLABEL_Y = 16;
var SEARCH_PAD_X = 26;
var SEARCH_PAD_Y = 8;
// dropdown popup (STAGE pixels -- popup lives on _root, not inside ui)
var POPUP_W = 210;
var POPUP_OPT_H = 22;
var POPUP_DROP_Y = 64;
var POPUP_ANCHOR_R = 180;
var POPUP_FLIP_PAD = 34;
var POPUP_BOTTOM = 445;
// tooltip paper: solid band is rows 30-78 of the 128px canvas
var TIP_BAND_C = 0.42;    // band CENTER as canvas fraction ((30+78)/2/128)
var TIP_BAND_F = 0.375;   // band HEIGHT as canvas fraction ((78-30)/128)
var TIP_MAXW = 620;       // wrap bound; longer tips wrap and the paper grows

var COL_LABEL = 0xE8DCC0;
var COL_VALUE = 0xD8B25A;
var COL_CAT = 0xC9A44C;
var COL_DARKTXT = 0x2E2416;
var COL_HINT = 0xB8AA88;
var COL_POPHOT = 0xFFE49A;

// ---------------------------------------------------------------- helpers --
function emit(n, a, b, c) {
    if (c != undefined) {
        flash.external.ExternalInterface.call(n, a, b, c);
    } else if (b != undefined) {
        flash.external.ExternalInterface.call(n, a, b);
    } else if (a != undefined) {
        flash.external.ExternalInterface.call(n, a);
    } else {
        flash.external.ExternalInterface.call(n);
    }
}

function toBool(v) {
    if (v == true || v == "true" || v == 1) {
        return true;
    }
    return false;
}

// numbers cross the engine as 32-bit floats (1.6 arrives as 1.6000000238419);
// snap everything to at most 3 decimals on ingestion
function num3(v) {
    return Math.round(Number(v) * 1000) / 1000;
}

// Text uses embedded per-style templates "mcm_t<size><l|c|r><rrggbb>":
// DefineEditText bound to the gfxfontlib import at the TAG level with size,
// alignment and color BAKED IN (compass.gfx pattern; embed.py TEXT_STYLES is
// the authoritative list -- both sides build the same name). AS only attaches
// a template and assigns .text. A style missing from TEXT_STYLES renders
// NOTHING (silently) -- keep the lists in sync. CORRECTED (2026-07-13): this
// used to claim runtime TextFormat/textColor is a no-op in this GFx player --
// disproven by analysis/CompassOverhaul/texttest.gfx (restyle works fine with
// .embedFonts = true set first). Kept as one template per style regardless;
// this table already works and there's no functional need to collapse it.
function mkText(mc, name, d, x, y, w, h, size, color, align) {
    var al = "l";
    if (align == "center") {
        al = "c";
    }
    if (align == "right") {
        al = "r";
    }
    var tpl = "mcm_t" + size + al + color.toString(16);
    var wrap = mc.attachMovie(tpl, name + "W", d);
    wrap._x = x;
    wrap._y = y;
    var t = wrap.t;
    t._width = w;
    t._height = h;
    mc[name] = t;
    return t;
}

// Largest baked size that keeps txt on ONE line. Runtime TextFormat is a
// no-op in the game's GFx player, so "shrinking" = swapping size templates --
// every size in the ladder must exist in embed.py TEXT_STYLES. The templates
// bake WordWrap in, so an overlong text grows DOWN: detect by textHeight.
function mkTextFit(mc, name, d, x, y, w, h, sizes, color, align, txt) {
    var t = null;
    for (var i = 0; i < sizes.length; i++) {
        t = mkText(mc, name, d, x, y + (sizes[0] - sizes[i]) / 2, w, h, sizes[i], color, align);
        t.text = txt;
        if (t.textHeight < sizes[i] * 1.6 || i == sizes.length - 1) {
            return t;
        }
        mc[name + "W"].removeMovieClip();
    }
    return t;
}

function fillRect(mc, w, h, color, alpha) {
    mc.beginFill(color, alpha);
    mc.moveTo(0, 0);
    mc.lineTo(w, 0);
    mc.lineTo(w, h);
    mc.lineTo(0, h);
    mc.endFill();
}

// standard hover feedback: dim the clip (or a sibling) while hovered
function hoverAlpha(clip, target, pct) {
    clip.hoverTarget = target;
    clip.hoverPct = pct;
    clip.onRollOver = function () { this.hoverTarget._alpha = this.hoverPct; };
    clip.onRollOut = function () { this.hoverTarget._alpha = 100; };
    clip.onDragOut = function () { this.hoverTarget._alpha = 100; };
}

// ------------------------------------------------------------------ stage --
var dim = _root.createEmptyMovieClip("dim", 10);
fillRect(dim, 800, 450, 0x000000, 55);

var ui = _root.createEmptyMovieClip("ui", 20);
ui._x = WIN_X;
ui._y = WIN_Y;
ui._xscale = WIN_SCALE;
ui._yscale = WIN_SCALE;

ui.attachMovie("mcm_main_window", "win", 1);

var titleTf = mkText(ui, "titleTf", 2, TITLE_X, TITLE_Y, TITLE_W, 60, 34, COL_DARKTXT);
titleTf.text = "Mod Configuration";

// -------------------------------------------------------- settings cache ---
// modCache[i] = {id, name, mc, items[]}; item = {kind, id, label, tip, value,
// def, options, min, max, step, suffix}. The panel is BUILT from the current
// mod's items; edits write back into the items, so switching away and back
// preserves state without any engine traffic.
var modCache = [];
var currentModId = "";

function findMod(id) {
    for (var i = 0; i < modCache.length; i++) {
        if (modCache[i].id == id) {
            return modCache[i];
        }
    }
    return null;
}

// ------------------------------------------------------------- mod list ----
var modsC = ui.createEmptyMovieClip("modsC", 5);
var modsView = modsC.createEmptyMovieClip("view", 1);
var modsRows = modsView.createEmptyMovieClip("rows", 1);
var modsMask = modsC.createEmptyMovieClip("mask", 2);
modsMask._x = MOD_X - 4;
modsMask._y = MOD_Y - 4;
fillRect(modsMask, MODROW_W + 24, MRAIL_H + 8, 0xFF0000, 100);
modsView.setMask(modsMask);

var mTrack = ui.attachMovie("mcm_vtrack_a", "mTrack", 6);
mTrack._x = MRAIL_X;
mTrack._y = MRAIL_Y;
mTrack._width = MRAIL_W;
mTrack._height = MRAIL_H;
var mKnob = ui.attachMovie("mcm_slider_knob", "mKnob", 7);
mKnob._width = KNOB_W;
mKnob._height = KNOB_H;
mKnob._x = MRAIL_X + MRAIL_W / 2 - KNOB_HALF;
var modScroll = 0;

function modsHeight() {
    return modCache.length * MODROW_H;
}

function setModScroll(y) {
    var maxS = modsHeight() - (MRAIL_H + 8);
    if (maxS < 0) {
        maxS = 0;
    }
    modScroll = y;
    if (modScroll < 0) {
        modScroll = 0;
    }
    if (modScroll > maxS) {
        modScroll = maxS;
    }
    modsRows._y = -modScroll;
    if (maxS > 0) {
        mKnob._visible = true;
        mKnob._y = MODKNOB_MIN + (MODKNOB_MAX - MODKNOB_MIN) * (modScroll / maxS);
    } else {
        mKnob._visible = false;
    }
}

mKnob.onPress = function () {
    this.grab = ui._ymouse - this._y;
    this.onMouseMove = function () {
        var maxS = modsHeight() - (MRAIL_H + 8);
        if (maxS < 0) {
            maxS = 0;
        }
        var t = (ui._ymouse - this.grab - MODKNOB_MIN) / (MODKNOB_MAX - MODKNOB_MIN);
        if (t < 0) {
            t = 0;
        }
        if (t > 1) {
            t = 1;
        }
        setModScroll(maxS * t);
    };
};
mKnob.onRelease = function () { delete this.onMouseMove; };
mKnob.onReleaseOutside = function () { delete this.onMouseMove; };

function setSelectedMod(id) {
    currentModId = id;
    for (var i = 0; i < modCache.length; i++) {
        var act = (modCache[i].id == id);
        modCache[i].mc.artA._visible = act;
        modCache[i].mc.artN._visible = !act;
    }
}

// ------------------------------------------------------------- content -----
var contentC = ui.createEmptyMovieClip("contentC", 8);

var headTf = mkText(contentC, "headTf", 1, HEAD_X, HEAD_Y, HEAD_W, 52, 40, COL_LABEL, "center");
headTf.text = "";

var sep = contentC.attachMovie("mcm_separator_c", "sep", 2);
sep._x = SEP_X;
sep._y = SEP_Y;
sep._width = SEP_W;
sep._height = SEP_H;

// search bar (right, below the big separator)
var searchC = contentC.createEmptyMovieClip("searchC", 3);
searchC._x = SEARCH_X;
searchC._y = SEARCH_Y;
var sArtN = searchC.attachMovie("mcm_search_normal", "artN", 1);
var sArtF = searchC.attachMovie("mcm_search_focus", "artF", 2);
sArtN._width = SEARCH_W;
sArtN._height = SEARCH_H;
sArtF._width = SEARCH_W;
sArtF._height = SEARCH_H;
sArtF._visible = false;
var searchTf = mkText(searchC, "inp", 3, SEARCH_PAD_X, SEARCH_PAD_Y, SEARCH_W - 100, SEARCH_H - 14, 26, 0xCFC2A0);
searchTf.type = "input";
searchTf.selectable = true;
var SEARCH_HINT = "Search...";
searchTf.text = SEARCH_HINT;
var searchFocused = false;   // gates the keybind hook while typing
searchTf.onSetFocus = function () {
    searchC.artF._visible = true;
    searchC.artN._visible = false;
    if (this.text == SEARCH_HINT) {
        this.text = "";
    }
    searchFocused = true;
};
searchTf.onKillFocus = function () {
    searchC.artF._visible = false;
    searchC.artN._visible = true;
    if (this.text == "") {
        this.text = SEARCH_HINT;
    }
    searchFocused = false;
};
searchTf.onChanged = function () { applyFilter(); };

// scrollable rows viewport (masked)
var rowsView = contentC.createEmptyMovieClip("rowsView", 4);
rowsView._x = VIEW_X;
rowsView._y = VIEW_Y;
var rowsC = rowsView.createEmptyMovieClip("rowsC", 1);
var rowsMask = contentC.createEmptyMovieClip("rowsMask", 5);
rowsMask._x = VIEW_X;
rowsMask._y = VIEW_Y;
fillRect(rowsMask, VIEW_W, VIEW_H, 0xFF0000, 100);
rowsView.setMask(rowsMask);

var rows = [];
var catCount = 0;
var scrollY = 0;
var hoverRow = null;   // settings row whose LABEL the cursor rests on (tooltip surface)

// --------------------------------------------------------------- scrollbar --
var sTrack = ui.attachMovie("mcm_vtrack_a", "sTrack", 9);
sTrack._x = SRAIL_X;
sTrack._y = SRAIL_Y;
sTrack._width = SRAIL_W;
sTrack._height = SRAIL_H;
var sbKnob = ui.attachMovie("mcm_slider_knob", "sbKnob", 10);
sbKnob._width = KNOB_W;
sbKnob._height = KNOB_H;
sbKnob._x = SRAIL_X + SRAIL_W / 2 - KNOB_HALF;
sbKnob._y = CONTENTKNOB_MIN;

function contentHeight() {
    var h = 0;
    for (var i = 0; i < rows.length; i++) {
        if (rows[i].shown) {
            h += rows[i].h;
        }
    }
    return h;
}

function setScroll(y) {
    var maxS = contentHeight() - VIEW_H;
    if (maxS < 0) {
        maxS = 0;
    }
    scrollY = y;
    if (scrollY < 0) {
        scrollY = 0;
    }
    if (scrollY > maxS) {
        scrollY = maxS;
    }
    rowsC._y = -scrollY;
    if (maxS > 0) {
        sbKnob._visible = true;
        sbKnob._y = CONTENTKNOB_MIN + (CONTENTKNOB_MAX - CONTENTKNOB_MIN) * (scrollY / maxS);
    } else {
        sbKnob._visible = false;
    }
}

sbKnob.onPress = function () {
    this.grab = ui._ymouse - this._y;
    this.onMouseMove = function () {
        var maxS = contentHeight() - VIEW_H;
        if (maxS < 0) {
            maxS = 0;
        }
        var t = (ui._ymouse - this.grab - CONTENTKNOB_MIN) / (CONTENTKNOB_MAX - CONTENTKNOB_MIN);
        if (t < 0) {
            t = 0;
        }
        if (t > 1) {
            t = 1;
        }
        setScroll(maxS * t);
    };
};
sbKnob.onRelease = function () { delete this.onMouseMove; };
sbKnob.onReleaseOutside = function () { delete this.onMouseMove; };

var WHEEL_SPLIT = (MOD_X + MODROW_W + 60) * WIN_SCALE / 100 + WIN_X;
var wheelHook = new Object();
// A textfield only fires onKillFocus when focus moves to another FOCUSABLE
// object -- clicking plain movieclips leaves the search box focused and
// searchFocused stuck true, killing the keybind hook for the session. Drop
// focus explicitly on any press outside the search area.
wheelHook.onMouseDown = function () {
    if (searchFocused) {
        if (!searchC.hitTest(_root._xmouse, _root._ymouse, false)) {
            Selection.setFocus(null);
            searchFocused = false;
        }
    }
};
wheelHook.onMouseWheel = function (delta) {
    hideTip();
    hoverRow = null;
    if (_root._xmouse < WHEEL_SPLIT) {
        setModScroll(modScroll - delta * WHEEL_STEP);
    } else {
        setScroll(scrollY - delta * WHEEL_STEP);
    }
};
Mouse.addListener(wheelHook);

// --------------------------------------------------------------- tooltip ---
// Art is the game's inventory paper strip: curled tail + stretchable paper +
// rolled end; dark text anchored to the solid band (TIP_BAND_C of the canvas).
var TIP_H = 76;   // minimum paper height (single-line tip)
var tipC = ui.createEmptyMovieClip("tipC", 40);
tipC._visible = false;
var tipL = tipC.attachMovie("mcm_tip_begin", "l", 1);
var tipM = tipC.attachMovie("mcm_tip_mid", "m", 2);
var tipR = tipC.attachMovie("mcm_tip_end", "r", 3);
// original art metrics; showTip rescales all three pieces per call (the
// paper height depends on how many lines the tip wraps into)
var TIP_ART_H = tipL._height;
var TIP_ART_WL = tipL._width;
var TIP_ART_WR = tipR._width;
var tipTf = mkText(tipC, "txt", 4, 0, 0, TIP_MAXW, 40, SIZE_TIP, COL_DARKTXT);
tipTf.autoSize = "left";

function showTip(txt) {
    if (txt == undefined || txt == "") {
        return;
    }
    tipTf._width = TIP_MAXW;   // wrap bound (template bakes WordWrap in)
    tipTf.text = txt;
    var tw = tipTf.textWidth + 12;
    // solid band (TIP_BAND_F of the canvas) must hold every wrapped line
    var H = (tipTf.textHeight + 14) / TIP_BAND_F;
    if (H < TIP_H) {
        H = TIP_H;
    }
    var sc = H / TIP_ART_H;
    tipL._width = TIP_ART_WL * sc;
    tipL._height = H;
    tipM._height = H;
    tipR._width = TIP_ART_WR * sc;
    tipR._height = H;
    var wL = tipL._width;
    var wR = tipR._width;
    tipM._x = wL - 2;
    tipM._width = tw + 4;
    tipR._x = wL + tw;
    tipTf._x = wL;
    tipTf._y = H * TIP_BAND_C - tipTf.textHeight / 2;
    var w = wL + tw + wR;
    // the curled tail's pointy tip inside tip_begin (art px 6,95) lands at
    // the cursor, band floats above-right; fall back to below-cursor when
    // the band would leave the window top
    var tx = ui._xmouse - 6 * sc;
    var ty = ui._ymouse - 95 * sc - 2;
    if (ty < 10) {
        ty = ui._ymouse + 24;
    }
    if (tx + w > 1560) {
        tx = 1560 - w;
    }
    if (tx < 4) {
        tx = 4;
    }
    if (ty + H > 950) {
        ty = ui._ymouse - H - 14;
    }
    tipC._x = tx;
    tipC._y = ty;
    tipC._visible = true;
}

function hideTip() {
    tipC._visible = false;
}

// arm/disarm from the per-row label hits; the ticker shows the tip only once
// the cursor has RESTED on the label for TIP_DELAY frames, and hides it again
// on any movement
var TIP_DELAY = 8;
var tipArmedText = "";
var tipArmed = false;
var tipTimer = 0;
var tipMX = 0;
var tipMY = 0;

function tipArm(txt) {
    tipArmedText = txt;
    tipArmed = true;
    tipTimer = 0;
    tipMX = _root._xmouse;
    tipMY = _root._ymouse;
    hideTip();
}

function tipDisarm() {
    tipArmed = false;
    hideTip();
}

var tipTicker = ui.createEmptyMovieClip("tipTicker", 41);
tipTicker.onEnterFrame = function () {
    if (!tipArmed) {
        return;
    }
    var dx = _root._xmouse - tipMX;
    var dy = _root._ymouse - tipMY;
    if (dx > 2 || dx < -2 || dy > 2 || dy < -2) {
        tipMX = _root._xmouse;
        tipMY = _root._ymouse;
        tipTimer = 0;
        hideTip();
        return;
    }
    if (tipTimer < TIP_DELAY) {
        tipTimer++;
        if (tipTimer == TIP_DELAY) {
            showTip(tipArmedText);
        }
    }
};

// ----------------------------------------------------------------- rows ----
function relayout() {
    var y = 0;
    for (var i = 0; i < rows.length; i++) {
        var r = rows[i];
        r.mc._visible = r.shown;
        if (r.shown) {
            r.mc._y = y;
            y += r.h;
        }
    }
    setScroll(scrollY);
}

function clearPanel() {
    for (var i = 0; i < rows.length; i++) {
        rows[i].mc.removeMovieClip();
    }
    rows = [];
    catCount = 0;
    scrollY = 0;
    hoverRow = null;
    lastSlider = null;                     // rows died; drop the key target
    keyRow = null;                         // cancel a held key WITHOUT emitting
    delete keyTicker.onEnterFrame;
    closePopup();
    hideTip();
    setScroll(0);
}

// shared row skeleton: clip, model, label, tooltip trigger. Per-kind builders
// add their control and refresher on top.
function rowBase(kind, it, h) {
    var i = rows.length;
    var mc = rowsC.createEmptyMovieClip("r" + i, 10 + i);
    var r = new Object();
    r.mc = mc;
    r.kind = kind;
    r.it = it;
    r.modId = currentModId;
    r.cat = catCount;
    r.h = h;
    r.shown = true;
    var lsize = SIZE_LABEL;
    var lcol = COL_LABEL;
    var lw = COL_SPLIT - 30;
    var lx = 0;
    var lalign = "left";
    var oneLine = 40;
    if (kind == "cat") {
        // category header is centered over its full-width separator
        lsize = SIZE_CAT;
        lcol = COL_CAT;
        lw = VIEW_W - 40;
        lx = 20;
        lalign = "center";
        oneLine = 52;
    }
    var lt = mkText(mc, "label", 1, lx, 6, lw, 52, lsize, lcol, lalign);
    lt.wordWrap = true;
    lt.multiline = true;
    lt.autoSize = lalign;
    lt.text = it.label;
    if (lt._height > oneLine) {
        r.h = r.h + 30;
    }
    // tooltip + reset target: ONLY the label text area (see tipTicker /
    // resetHovered) -- not the whole row, not while the cursor is moving
    var lh = mc.createEmptyMovieClip("labelHit", 8);
    lh._x = lx;
    lh._y = 4;
    fillRect(lh, lt.textWidth + 14, lt._height + 8, 0x000000, 0);
    lh.row = r;
    lh.onRollOver = function () {
        hoverRow = this.row;
        if (this.row.kind == "slider") {
            lastSlider = this.row;         // arrow-key target follows the hover
        }
        tipArm(this.row.it.tip);
    };
    lh.onRollOut = function () {
        hoverRow = null;
        tipDisarm();
    };
    lh.onDragOut = function () {
        hoverRow = null;
        tipDisarm();
    };
    if (kind == "cat") {
        // only categories keep a separator; option rows flow uninterrupted
        var sp = mc.attachMovie("mcm_separator_thin", "sep", 3);
        sp._x = 0;
        sp._y = h - 16;
        sp._width = VIEW_W;
        sp._height = 22;
    }
    rows.push(r);
    return r;
}

// Outbound args cross the EI typed path intact (numbers arrive as float
// TUIData -- the old "transport kills args" saga was a plugin-side tag-read
// bug, see analysis/MCM/ei_transport_investigation.md). Indices mirror the
// C++ model: mod = push order, item = per-mod arrival order incl. categories.
function emitValue(r) {
    var m = findMod(r.modId);
    if (m == null) {
        return;
    }
    emit("onValueChanged", m.idx, r.it.idx, r.it.value);
}

// category --------------------------------------------------------------
function buildCategoryRow(it) {
    catCount++;
    rowBase("cat", it, CAT_H);
}

// toggle ----------------------------------------------------------------
function refreshToggle(r) {
    r.mc.box.on._visible = r.it.value;
    r.mc.box.off._visible = !r.it.value;
}

function buildToggleRow(it) {
    var r = rowBase("toggle", it, ROW_H);
    var box = r.mc.createEmptyMovieClip("box", 5);
    box._x = CTRL_R - TOGGLE_X_INSET;
    box._y = 6;
    var on = box.attachMovie("mcm_checkbox_on", "on", 1);
    var off = box.attachMovie("mcm_checkbox_off", "off", 2);
    on._width = 52;
    on._height = 52;
    off._width = 52;
    off._height = 52;
    box.row = r;
    refreshToggle(r);
    hoverAlpha(box, box, 80);
    box.onPress = function () {
        this.row.it.value = !this.row.it.value;
        refreshToggle(this.row);
        emitValue(this.row);
    };
}

// dropdown ---------------------------------------------------------------
var popupOpen = false;

function closePopup() {
    if (popupOpen) {
        _root.popup.removeMovieClip();
        _root.popCatch.removeMovieClip();
        popupOpen = false;
    }
}

function refreshDropdown(r) {
    r.mc.val.text = r.it.options[r.it.value];
}

function openPopup(r) {
    closePopup();
    var catcher = _root.createEmptyMovieClip("popCatch", 4990);
    fillRect(catcher, 800, 450, 0x000000, 0);
    catcher.onPress = function () { closePopup(); };

    var popup = _root.createEmptyMovieClip("popup", 5000);
    var opts = r.it.options;
    var PH = opts.length * POPUP_OPT_H + 14;
    var p = new Object();
    p.x = r.mc.box._x;
    p.y = r.mc.box._y + POPUP_DROP_Y;
    r.mc.localToGlobal(p);
    if (p.y + PH > POPUP_BOTTOM) {
        p.y -= (PH + POPUP_FLIP_PAD);
    }
    var px = p.x + POPUP_ANCHOR_R - POPUP_W;
    if (px > 800 - POPUP_W - 4) {
        px = 800 - POPUP_W - 4;
    }
    popup._x = px;
    popup._y = p.y;
    var bgArt = popup.attachMovie("mcm_panel_dark", "bg", 1);
    bgArt._width = POPUP_W;
    bgArt._height = PH;
    for (var i = 0; i < opts.length; i++) {
        var it = popup.createEmptyMovieClip("o" + i, 10 + i);
        it._x = 8;
        it._y = 7 + i * POPUP_OPT_H;
        fillRect(it, POPUP_W - 16, POPUP_OPT_H, 0x000000, 0);
        // three baked color states stacked (runtime textColor is as dead in
        // the game's GFx as TextFormat): 0 normal / 1 selected / 2 hover
        var tN = mkText(it, "tN", 1, 4, 2, POPUP_W - 24, 18, 13, COL_LABEL);
        var tS = mkText(it, "tS", 2, 4, 2, POPUP_W - 24, 18, 13, COL_VALUE);
        var tH = mkText(it, "tH", 3, 4, 2, POPUP_W - 24, 18, 13, COL_POPHOT);
        tN.text = opts[i];
        tS.text = opts[i];
        tH.text = opts[i];
        it.idx = i;
        it.row = r;
        it.setState = function (s) {
            this.tN._parent._visible = (s == 0);
            this.tS._parent._visible = (s == 1);
            this.tH._parent._visible = (s == 2);
        };
        it.baseState = 0;
        if (i == r.it.value) {
            it.baseState = 1;
        }
        it.setState(it.baseState);
        it.onRollOver = function () { this.setState(2); };
        it.onRollOut = function () { this.setState(this.baseState); };
        it.onDragOut = it.onRollOut;
        it.onPress = function () {
            var r = this.row;              // closePopup() destroys `this` --
            r.it.value = this.idx;         // grab the row first
            refreshDropdown(r);
            closePopup();
            emitValue(r);
        };
    }
    popupOpen = true;
}

function buildDropdownRow(it) {
    var r = rowBase("dropdown", it, ROW_H);
    // value text and arrow share the label's top edge (label y=6, 2px field
    // gutter puts glyph tops at ~8 -- arrow art top matches at 8)
    var vt = mkText(r.mc, "val", 4, COL_SPLIT + 13, 6, 290, 40, SIZE_VALUE, COL_VALUE, "right");
    var ar = r.mc.attachMovie("mcm_arrow_down", "arrow", 5);
    ar._x = CTRL_R - ARROW_X_INSET;
    ar._y = 8;
    ar._width = 38;
    ar._height = 24;
    var box = r.mc.createEmptyMovieClip("box", 6);
    box._x = COL_SPLIT;
    box._y = 0;
    fillRect(box, CTRL_R - COL_SPLIT, 64, 0x000000, 0);
    box.row = r;
    hoverAlpha(box, ar, 70);
    box.onPress = function () { openPopup(this.row); };
    r.mc.box = box;
    refreshDropdown(r);
}

// slider -----------------------------------------------------------------
function refreshSlider(r) {
    r.mc.val.text = String(r.it.value) + r.it.suffix;
    r.mc.knob._x = r.knobMinX + (r.knobMaxX - r.knobMinX) *
                   ((r.it.value - r.it.min) / (r.it.max - r.it.min));
    r.mc.knob._y = 14;
}

// Visual-only during a drag: the value/knob track the mouse live, but the
// onValueChanged emit waits for release (endDrag) so broadcasts don't spam
// per-pixel updates.
function sliderFromMouse(r) {
    var t = (r.mc._xmouse - r.knobMinX - KNOB_HALF) / (r.knobMaxX - r.knobMinX);
    if (t < 0) {
        t = 0;
    }
    if (t > 1) {
        t = 1;
    }
    var v = r.it.min + t * (r.it.max - r.it.min);
    v = Math.round(v / r.it.step) * r.it.step;
    v = Math.round(v * 1000) / 1000;   // kill float noise; at most 3 decimals
    if (v != r.it.value) {
        r.it.value = v;
        refreshSlider(r);
    }
}

// visual-only single step; like the drag, the emit waits until the
// interaction ends (one broadcast per keypress OR per hold)
function stepSlider(r, dir) {
    var v = r.it.value + dir * r.it.step;
    v = Math.round(v / r.it.step) * r.it.step;
    v = Math.round(v * 1000) / 1000;
    if (v < r.it.min) {
        v = r.it.min;
    }
    if (v > r.it.max) {
        v = r.it.max;
    }
    if (v != r.it.value) {
        r.it.value = v;
        refreshSlider(r);
    }
}

// keyboard <-/-> tweak the LAST SLIDER the cursor touched (label hover or
// knob drag). C++ feeds left_down/left_up/right_down/right_up through
// SetInput; steps repeat while the key is held (~0.5s arm delay, then a
// step every 2 frames) and the single onValueChanged emit fires on key-up
// -- same batching as the knob drag.
var STEP_HOLD_DELAY = 15;
var lastSlider = null;
var keyRow = null;
var keyStartValue = 0;
var keyTicker = ui.createEmptyMovieClip("keyTicker", 42);

function keyStepStart(dir) {
    if (keyRow != null) {          // opposite arrow while one is held
        keyStepEnd();
    }
    if (lastSlider == null || !lastSlider.shown) {
        return;
    }
    keyRow = lastSlider;
    keyStartValue = keyRow.it.value;
    stepSlider(keyRow, dir);
    keyTicker.hold = 0;
    keyTicker.dir = dir;
    keyTicker.onEnterFrame = function () {
        this.hold++;
        if (this.hold > STEP_HOLD_DELAY && this.hold % 2 == 0) {
            stepSlider(keyRow, this.dir);
        }
    };
}

function keyStepEnd() {
    delete keyTicker.onEnterFrame;
    if (keyRow != null && keyRow.it.value != keyStartValue) {
        emitValue(keyRow);
    }
    keyRow = null;
}

function buildSliderRow(it) {
    var r = rowBase("slider", it, ROW_H_SLIDER);
    var TX = CTRL_R - SLIDER_W;
    var tr = r.mc.attachMovie("mcm_htrack_c", "track", 4);
    tr._x = TX;
    tr._y = 18;
    tr._width = SLIDER_W;
    tr._height = 36;
    var kn = r.mc.attachMovie("mcm_slider_knob", "knob", 5);
    kn._width = KNOB_W;
    kn._height = KNOB_H;
    // value sits on its own line, centered under the middle of the track
    mkText(r.mc, "val", 6, TX, 58, SLIDER_W, 30, SIZE_VALUE, COL_VALUE, "center");
    r.knobMinX = TX + SLIDER_INSET_L;
    r.knobMaxX = TX + SLIDER_W - SLIDER_INSET_R;
    refreshSlider(r);
    var hit = r.mc.createEmptyMovieClip("hit", 7);
    hit._x = TX;
    hit._y = 4;
    fillRect(hit, SLIDER_W, 66, 0x000000, 0);
    hit.row = r;
    hit.endDrag = function () {
        delete this.onMouseMove;
        if (this.row.it.value != this.startValue) {
            emitValue(this.row);          // one update per drag, on release
        }
    };
    hit.onPress = function () {
        lastSlider = this.row;             // arrow-key target follows the drag
        this.startValue = this.row.it.value;
        sliderFromMouse(this.row);
        this.onMouseMove = function () { sliderFromMouse(this.row); };
    };
    hit.onRelease = function () { this.endDrag(); };
    hit.onReleaseOutside = function () { this.endDrag(); };
}

// ------------------------------------------------------------ panel build --
function showMod(id) {
    var m = findMod(id);
    if (m == null) {
        return;
    }
    setSelectedMod(id);
    clearPanel();
    headTf.text = m.name;
    for (var i = 0; i < m.items.length; i++) {
        var it = m.items[i];
        if (it.kind == "cat") {
            buildCategoryRow(it);
        } else if (it.kind == "toggle") {
            buildToggleRow(it);
        } else if (it.kind == "dropdown") {
            buildDropdownRow(it);
        } else if (it.kind == "slider") {
            buildSliderRow(it);
        }
    }
    applyFilter();
}

// -------------------------------------------------------------- filtering --
function applyFilter() {
    var q = searchTf.text.toLowerCase();
    if (q == SEARCH_HINT.toLowerCase()) {
        q = "";
    }
    var i;
    var r;
    if (q == "") {
        for (i = 0; i < rows.length; i++) {
            rows[i].shown = true;
        }
    } else {
        var catHas = new Object();
        for (i = 0; i < rows.length; i++) {
            r = rows[i];
            r.selfMatch = (r.it.label.toLowerCase().indexOf(q) >= 0 ||
                           r.it.tip.toLowerCase().indexOf(q) >= 0);
            if (r.kind != "cat" && r.selfMatch) {
                catHas[r.cat] = true;
            }
        }
        for (i = 0; i < rows.length; i++) {
            r = rows[i];
            if (r.kind == "cat") {
                r.shown = (r.selfMatch || catHas[r.cat] == true);
            } else {
                r.shown = r.selfMatch;
            }
        }
    }
    scrollY = 0;
    relayout();
}

// ----------------------------------------------------------------- hints ---
// Key chips group at the far LEFT; Esc sits alone at the far RIGHT. The X
// chip is a display-only keybind hint (a click can't carry cursor context);
// Esc is clickable (Back needs none). `key` may be an array -- consecutive
// chips share one label. Arrow keys follow the game's own convention (no
// arrow glyph art exists; Menu.gfx puts the localized key NAME on the
// mid-width keycap frame, see ControlsEnum.as).
function addHint(idx, key, label, x, clickable, ev) {
    var c = hintsC.createEmptyMovieClip("h" + idx, 10 + idx);
    c._x = x;
    var keys = (key instanceof Array) ? key : [key];
    var cx = 0;
    for (var i = 0; i < keys.length; i++) {
        var chipName = "mcm_key_sq";
        var chipW = 18;
        if (keys[i].length > 1) {
            chipName = "mcm_key_mid";
            chipW = 30;
        }
        var chip = c.attachMovie(chipName, "chip" + i, 1 + i * 2);
        chip._x = cx;
        chip._width = chipW;
        chip._height = 18;
        var kt = mkText(c, "key" + i, 2 + i * 2, cx, 3, chipW, 13, 9, COL_DARKTXT, "center");
        kt.text = keys[i];
        cx += chipW + 4;
    }
    var lt = mkText(c, "lbl", 30, cx + 2, 3, 130, 15, 11, COL_HINT);
    lt.text = label;
    if (clickable) {
        c.ev = ev;
        c.onPress = function () { emit(this.ev); };
    }
}

var hintsC = _root.createEmptyMovieClip("hintsC", 6000);
hintsC._y = HINT_Y;
addHint(0, "X", "Reset Setting", HINT_X0, false, "");
addHint(1, ["Left", "Right"], "Adjust Slider", HINT_X1, false, "");
addHint(2, "Esc", "Back", HINT_X3, false, "");   // display-only (Esc is native)

// Resets exactly ONE setting: the one whose label text the cursor is resting
// on -- the tooltip surface. Resolved by hitTest at keypress time; the
// rollOver-driven hoverRow is only a fallback (edge events go stale after
// rebuilds/scrolls, and rollOver may not re-fire without cursor motion).
function hoveredSettingRow() {
    if (hoverRow != null && hoverRow.kind != "cat") {
        return hoverRow;
    }
    if (!rowsMask.hitTest(_root._xmouse, _root._ymouse, false)) {
        return null;
    }
    for (var i = 0; i < rows.length; i++) {
        var r = rows[i];
        if (!r.shown || r.kind == "cat") {
            continue;
        }
        if (r.mc.labelHit.hitTest(_root._xmouse, _root._ymouse, true)) {
            return r;
        }
    }
    return null;
}

function resetHovered() {
    var r = hoveredSettingRow();
    if (r == null) {
        return;
    }
    if (r.it.value == r.it.def) {
        return;
    }
    r.it.value = r.it.def;
    if (r.kind == "toggle") {
        refreshToggle(r);
    } else if (r.kind == "dropdown") {
        refreshDropdown(r);
    } else if (r.kind == "slider") {
        refreshSlider(r);
    }
    emitValue(r);
}

// ------------------------------------------------------------ engine API ---
function fc_beginMods() {
    for (var i = 0; i < modCache.length; i++) {
        modCache[i].mc.removeMovieClip();
    }
    modCache = [];
    currentModId = "";
    setModScroll(0);
    clearPanel();
    headTf.text = "";
}

function fc_addMod(modId, uiName) {
    var m = new Object();
    m.id = modId;
    m.name = uiName;
    m.items = [];
    var i = modCache.length;
    m.idx = i;   // == index in the plugin's g_mods (push order)
    var mc = modsRows.createEmptyMovieClip("m" + i, 10 + i);
    mc._x = MOD_X;
    mc._y = MOD_Y + i * MODROW_H;
    var a = mc.attachMovie("mcm_list_row_active", "artA", 1);
    var n = mc.attachMovie("mcm_list_row_normal", "artN", 2);
    a._width = MODROW_W;
    a._height = MODROW_H;
    n._width = MODROW_W;
    n._height = MODROW_H;
    a._visible = false;
    mkTextFit(mc, "label", 3, MODLABEL_X, MODLABEL_Y, MODROW_W - 90, 40, [28, 22, 17], COL_DARKTXT, "left", uiName);
    m.mc = mc;
    modCache.push(m);
    mc.modId = modId;
    hoverAlpha(mc, mc, 85);
    mc.onPress = function () { showMod(this.modId); };
    setModScroll(modScroll);
}

function fc_addCategory(modId, label, tip) {
    var m = findMod(modId);
    if (m == null) {
        return;
    }
    var it = new Object();
    it.kind = "cat";
    it.id = "";
    it.label = label;
    it.tip = tip;
    it.idx = m.items.length;   // == index in the plugin's items vector
    m.items.push(it);
}

function fc_addToggle(modId, id, label, tip, value, def) {
    var m = findMod(modId);
    if (m == null) {
        return;
    }
    var it = new Object();
    it.kind = "toggle";
    it.id = id;
    it.label = label;
    it.tip = tip;
    it.value = toBool(value);
    it.def = toBool(def);
    it.idx = m.items.length;
    m.items.push(it);
}

function fc_addDropdown(modId, id, label, tip, optionsPipe, index, defIndex) {
    var m = findMod(modId);
    if (m == null) {
        return;
    }
    var it = new Object();
    it.kind = "dropdown";
    it.id = id;
    it.label = label;
    it.tip = tip;
    it.options = String(optionsPipe).split("|");
    it.value = Number(index);
    it.def = Number(defIndex);
    it.idx = m.items.length;
    m.items.push(it);
}

function fc_addSlider(modId, id, label, tip, minV, maxV, step, value, def, suffix) {
    var m = findMod(modId);
    if (m == null) {
        return;
    }
    var it = new Object();
    it.kind = "slider";
    it.id = id;
    it.label = label;
    it.tip = tip;
    it.min = num3(minV);
    it.max = num3(maxV);
    it.step = num3(step);
    it.value = num3(value);
    it.def = num3(def);
    if (suffix == undefined) {
        it.suffix = "";
    } else {
        it.suffix = String(suffix);
    }
    it.idx = m.items.length;
    m.items.push(it);
}

// Keybind feed from the plugin's native input listener (mirrors the game's
// Menu.gfx SetInput pattern). Flash owns the search-focus semantics: "back"
// while typing just leaves the search box; otherwise it emits onBack (no-arg
// event -- names never die in the transport) and the plugin closes the menu.
function fc_setInput(action) {
    if (action == "back") {
        if (searchFocused) {
            Selection.setFocus(null);
            searchFocused = false;
            return;
        }
        emit("onBack");
        return;
    }
    if (action == "left_up" || action == "right_up") {
        keyStepEnd();   // NOT gated: a hold must always end, even if the
        return;         // search box grabbed focus mid-hold
    }
    if (searchFocused) {
        return;   // arrows move the caret while typing
    }
    if (action == "reset") {
        resetHovered();
    } else if (action == "left_down") {
        keyStepStart(-1);
    } else if (action == "right_down") {
        keyStepStart(1);
    }
}

function fc_showMods() {
    if (modCache.length == 0) {
        clearPanel();
        headTf.text = "No mods installed";
        return;
    }
    showMod(modCache[0].id);
}

stop();
