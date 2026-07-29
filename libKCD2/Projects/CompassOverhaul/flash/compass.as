// CompassOverhaul :: compass.gfx  (ActionScript 2 / Scaleform GFx)
// ---------------------------------------------------------------------------
// Self-driven HUD overlay, loaded by the plugin into _root.compassOverlay of the
// game's "hud" movie. Reads the native compass data directly:
//   _root.g_MarkersO  dict (Enum.MARKER+id) -> CompassMarker (m_Mc, m_Distance, ...)
//
// TUNING: edit the C_* constants below and rebuild (build_gfx.sh). Text size
// is authored in base.xml (DefineEditTextTag fontHeight, twips = px * 20).
//
// TEXT (authentic game pattern): both labels are AUTHORED DefineEditTexts bound
// to the ImportAssets2-imported "DefaultFont" from gfxfontlib.gfx -- the exact
// mechanism hud.gfx uses (sprite prototype + instance "tField"; see base.xml).
// AS only attachMovie()s them and sets .text here. CORRECTED (2026-07-13): an
// earlier version of this comment claimed runtime TextFormat.font/size/color
// changes are ignored by GFx -- that's wrong, disproven by
// analysis/CompassOverhaul/texttest.gfx: TextFormat correctly restyles both
// fresh createTextField() fields and already tag-bound fields like these,
// PROVIDED .embedFonts = true is set first (the real missing ingredient
// behind the old belief -- unset, a field silently falls back to
// non-embedded/device rendering, indistinguishable from "the restyle did
// nothing"). Left baked here anyway: this template predates the finding and
// already works, so there's no functional reason to change it.
//
// NAMES (all strings arrive PRE-LOCALIZED -- the GFx translator does not
// resolve "@keys" set at runtime in this movie, so the plugin localizes
// natively via sub_1803C235C and pushes display text as flat string arrays).
// Both tables are pushed onto this clip from quest-marker events ONLY (the
// C_UIQuestLog::AddObjectiveCompassMarkers hook, 0xDC5F24), which fire after
// the queued loadMovie has completed -- an earlier push would be wiped when
// the load replaces this clip's content:
//   co_names     = [markerId, name, ...]  quest objectives
//   co_typeNames = [type, name, ...]      every mark type with a
//                  "ui_maplegend_<type>" loc entry (the map-tooltip key);
//                  types without one are absent -> their name label hides
// Both arrays only ever grow, so a length change is the reparse signal.
//
// TARGETING: a marker is "aimed" when its global x is near the screen centre
// (the native code scrolls markers so the faced bearing lands at centre).
// Markers at the SAME bearing but different distances STACK at the same x, so
// the grow is applied to EVERY marker inside the aim window (fixes the desync
// where the enlarged marker was a hidden one behind the visible icon). The
// label follows the TOP-MOST in-window marker; alpha-0 (far-culled) and locked
// markers are skipped -- g_MarkersO holds ALL registered markers, not just
// visible ones.
//
// SCALING: native CompassMarker.updateMC() owns m_Mc._xscale (distance-based);
// we scale m_Mc.inside (icon+char+arrow), which native never touches, so the
// aimed marker renders at native_scale * 1.325 with no tug-of-war.
// ---------------------------------------------------------------------------

stop();

var C_AIM_FRAC  = 0.006;   // aim window half-width, fraction of screen width
var C_AIM_SCALE = 132.5;   // aimed inner-group scale (Skyrim's x1.325); 100 = native
var C_LABEL_DY  = -46;     // distance label offset above the marker, px
var C_NAME_DY   = 25;      // name label offset BELOW the marker, px

var self = this;
self.attachMovie("DistLabel", "lbl", self.getNextHighestDepth());
self.lbl._visible = false;
self.attachMovie("NameLabel", "nameLbl", self.getNextHighestDepth());
self.nameLbl._visible = false;

// Measure each field's authored centre inside its sprite, then let autoSize
// "center" keep that centre anchored while the field hugs the text -- the
// label sprite can then be placed by centre, independent of text length or
// the field's own alignment.
var lblCx  = self.lbl.tField._x + self.lbl.tField._width / 2;
var nameCx = self.nameLbl.tField._x + self.nameLbl.tField._width / 2;
self.lbl.tField.autoSize = "center";
self.nameLbl.tField.autoSize = "center";

var namesTable     = {};
var lastNamesLen   = -1;
var typeNamesTable = {};
var lastTypesLen   = -1;

function parsePairs(arr)
{
   var o = {};
   for (var i = 0; i + 1 < arr.length; i += 2)
   {
      o[arr[i]] = arr[i + 1];
   }
   return o;
}

function isShown(mc)
{
   var o = mc;
   while (o != undefined && o != _root)
   {
      if (o._visible == false || o._alpha <= 0) return false;
      o = o._parent;
   }
   return true;
}

self.onEnterFrame = function()
{
   var na = self.co_names;
   if (na != undefined && na.length != lastNamesLen)
   {
      lastNamesLen = na.length;
      namesTable = parsePairs(na);
   }
   var ta = self.co_typeNames;
   if (ta != undefined && ta.length != lastTypesLen)
   {
      lastTypesLen = ta.length;
      typeNamesTable = parsePairs(ta);
   }

   var mo = _root.g_MarkersO;
   if (mo == undefined) { self.lbl._visible = false; self.nameLbl._visible = false; return; }

   var cx  = Stage.width / 2;
   var thr = Stage.width * C_AIM_FRAC;

   var tgtKey    = undefined;
   var bestDepth = undefined;
   var tgtPt     = undefined;

   for (var key in mo)
   {
      var m  = mo[key];
      var mc = m.m_Mc;
      if (mc == undefined || mc.inside == undefined) continue;
      // skip anything not actually on screen (own fade OR hidden ancestor)
      if (!isShown(mc)) continue;
      if (m.m_IsLocked) continue;   // locked markers: m_Distance forced to 0, spread at fixed offsets

      var p = {x: 0, y: 0};
      mc.localToGlobal(p);
      var dx = Math.abs(p.x - cx);

      // grow EVERY marker inside the aim window (stacked markers share one x)
      var sc = (dx < thr) ? C_AIM_SCALE : 100;
      mc.inside._xscale = sc;
      mc.inside._yscale = sc;

      // label the TOP-MOST in-window marker (nearer markers get higher depths
      // via getMCDepth) -- that is the icon the player actually sees grown
      if (dx < thr)
      {
         var d = mc.getDepth();
         if (bestDepth == undefined || d > bestDepth) { bestDepth = d; tgtKey = key; tgtPt = p; }
      }
   }

   if (tgtKey == undefined) { self.lbl._visible = false; self.nameLbl._visible = false; return; }

   self.globalToLocal(tgtPt);
   self.lbl._x = tgtPt.x - lblCx;         // field centre -> icon centre
   self.lbl._y = tgtPt.y + C_LABEL_DY;
   self.lbl.tField.text = Math.round(mo[tgtKey].m_Distance) + " m";
   self.lbl._visible = true;

   var nm = namesTable[tgtKey];           // localized objective name from the plugin
   if (nm == undefined) nm = typeNamesTable[mo[tgtKey].m_Type];
   if (nm == undefined)
   {
      self.nameLbl._visible = false;
   }
   else
   {
      self.nameLbl._x = tgtPt.x - nameCx; // field centre -> icon centre
      self.nameLbl._y = tgtPt.y + C_NAME_DY;   // below the icon
      self.nameLbl.tField.text = nm;      // pre-localized by the plugin
      self.nameLbl._visible = true;
   }
};
