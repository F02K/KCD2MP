// Floating Damage :: floatingdmg.gfx  (ActionScript 2 / Scaleform GFx)
// ---------------------------------------------------------------------------
// Self-contained popup spawner. Loaded by the plugin into a container clip
// inside the game's "hud" movie -- same injection pattern as CompassOverhaul:
// CFlashUIElement::Init hook -> CreateEmptyMovieClip -> loadMovie(this file).
//
// Native entry point: this.ShowDamage(amount, isHealthDmg, xPx, yPx, sizePx, isFatal)
//   amount      Number   signed delta, shown rounded. amount < 0 on a HEALTH
//               change means healing (design choice made here, not an RE'd
//               native convention -- caller must pass a negative amount for
//               heals; see COLOR SELECTION below).
//   isHealthDmg Boolean  true = this is a HEALTH change (damage or heal),
//               false = STAMINA damage. Renamed from the old isCrit -- color
//               is now driven by which resource changed, not crit-ness.
//   xPx     Number   absolute stage pixel X (screen-space anchor)
//   yPx     Number   absolute stage pixel Y
//   Deliberately NOT fractions of Stage.width/height -- the caller (native
//   side) always has the real renderer viewport size on hand, so passing
//   raw pixels sidesteps ever needing Stage.width/height here to agree with
//   it (GFx's Stage dimensions for a loadMovie'd child are not guaranteed to
//   track the live render target 1:1).
//   sizePx  Number   text point size, native-computed (scaled by damage
//               amount, MCM-configurable min/max) -- this side just applies
//               whatever it's given via TextFormat, no clamping/scaling here.
//   isFatal Boolean  true = this hit killed the victim (native-computed, see
//               plugin.cpp's IsDeadByHealth() check) -- draws SkullIcon next
//               to the number. skull.png is pre-colored red (build.py chroma-
//               keys its black backdrop to transparent) -- not retinted here.
//               Only meaningful alongside isHealthDmg; ignored otherwise.
//
// COLOR SELECTION:
//   isHealthDmg && amount >= 0  -> C_COLOR_HEALTH_DMG  (red)    health damage
//   isHealthDmg && amount <  0  -> C_COLOR_HEAL        (green) health healed
//   !isHealthDmg                 -> C_COLOR_STAMINA_DMG (yellow) stamina damage
//   (no stamina-heal color -- wasn't asked for; add one here if regen ever needs it)
//
// No world-tracking yet: caller supplies a plain screen-space anchor per spawn,
// set once at attach time. Swapping in per-frame native reprojection (WUID ->
// entity -> screen pos, mirroring IRenderer::ProjectToScreen -- see
// analysis/damage_numbers/worldtoscreen_hud_anchoring.md) is a drop-in addition
// to spawnPopup()'s onEnterFrame once that native call exists; skse-plugins.
// FloatingDamageV2.WorldPtToScreenPt3 in the Skyrim source is the model to
// follow (analysis/damage_numbers/damage_pipeline.md).
//
// TEXT: ONE tag-authored DefineEditText shared by all 5 anims (font baked at
// the tag level, color/size NOT baked -- see build.py). Both are applied live
// via TextFormat right after attach. This used to be 2 baked color variants x
// 5 anims (10 sprites), on the assumption that GFx ignores runtime TextFormat
// restyle on tag-bound fields; disproven by
// analysis/CompassOverhaul/texttest.gfx (restyle works fine, PROVIDED
// .embedFonts = true is set on the field first -- that was the actual
// missing ingredient behind the old belief, not a hard per-color-template
// requirement).
//
// LIFECYCLE: each popup is its own attached "<Anim>" clip; it removes itself
// once its baked pop-in+fade timeline finishes (_currentframe ==
// _totalframes) -- lifetime and motion are authored per-animation in the .gfx
// (see analysis/Floating Damage/build.py), not timed here.
//
// HIERARCHY: "<Anim>" (e.g. AccelerateIns) -> anim_ -> ins_ (+ skull_ when fatal).
// The baked per-frame curve targets anim_, not ins_ directly -- so ins_ and
// skull_ (attached inside anim_, not inside the outer "<Anim>" clip) both ride
// the same pop-in/scale/fade for free, with no per-frame ActionScript sync.
// ---------------------------------------------------------------------------

stop();

var C_ANIM_HEALTHDMG  = "AccelerateIns";  // pop-in style for a health hit (punchier rise)
var C_ANIM_STAMINADMG = "DropIns";        // pop-in style for a stamina hit
var C_COLOR_HEALTH_DMG  = 0xFF3B3B;   // red    -- damage to health
var C_COLOR_STAMINA_DMG = 0xF0F299;   // yellow -- damage to stamina
var C_COLOR_HEAL         = 0x07F236;  // green  -- healing (health gain)
var C_FALLBACK_TEXT_SIZE = 32;        // px; only used if a caller omits sizePx entirely

// Fatal-hit skull icon (SkullIcon, see build.py): sized/placed relative to the
// number's own text size so it scales together with the damage-proportionate text.
var C_SKULL_HEIGHT_SCALE = 1.1;   // icon height relative to sizePx
var C_SKULL_GAP_SCALE    = 0.5;   // extra gap right of the field's measured right edge, relative to sizePx (was offset-from-center; retune -- likely too big now, see spawnPopup)
var C_SKULL_Y_OFFSET_PX     = -3;     // extra offset down from the field's measured vertical center, in pixels (was 0; retune -- likely too low now, see spawnPopup)

var self = this;

function spawnPopup(linkageName, text, colorHex, sizePx, xPx, yPx, isFatal)
{
   var depth = self.getNextHighestDepth();
   var mc = self.attachMovie(linkageName, "dmg" + depth, depth);

   // anim_ is the wrapper the baked per-frame curve actually targets (see build.py);
   // ins_ lives inside it. Both ins_ and (when fatal) skull_ inherit anim_'s
   // pop-in/scale/fade automatically -- no per-frame ActionScript sync needed.
   var anim = mc.anim_;
   var field = anim.ins_;

   field.wordWrap = false;
   field.multiline = false;
   field.autoSize = "center";   // keep the field centered on the anchor as digit count varies
   field.embedFonts = true;     // required for the TextFormat restyle below to take on a tag-bound field
   var fmt = new TextFormat();
   fmt.color = colorHex;
   fmt.size = sizePx;
   field.setNewTextFormat(fmt);
   field.text = text;
   field.setTextFormat(fmt);
   mc._x = xPx;
   mc._y = yPx;

   if (isFatal)
   {
      var bounds = anim.getBounds(anim);
      
      // SkullIcon is centered on its own local (0,0) (see build.py), same convention
      // as ins_'s autoSize="center" -- so _x/_y here anchor its center, not a corner.
      // Attached inside anim_ (not mc) and after ins_ in depth order (renders on top
      // of it), right of it on the x axis.
      var sk = anim.attachMovie("SkullIcon", "skull_", anim.getNextHighestDepth());
      var nativeH = sk._height;   // read BEFORE any scale override -- native pixel height at the SWF's authored 100% scale
      if (nativeH > 0)
      {
         var scalePct = (sizePx * C_SKULL_HEIGHT_SCALE / nativeH) * 100;
         sk._xscale = scalePct;
         sk._yscale = scalePct;
      }
      // field's ACTUAL rendered bounds, in anim's coordinate space (same space
      // skull_ lives in) -- not assumed from local (0,0). autoSize="center" is only
      // confirmed to keep the field horizontally centered there (tested via varying
      // digit counts); its vertical behavior was never verified, so this doesn't
      // lean on that assumption for the y axis either.
      
      sk._x = bounds.xMax + sizePx * C_SKULL_GAP_SCALE;
      sk._y = (bounds.yMin + bounds.yMax) / 2 + C_SKULL_Y_OFFSET_PX;
   }

   mc.onEnterFrame = function()
   {
      if (this._currentframe == this._totalframes)
      {
         delete this.onEnterFrame;
         this.removeMovieClip();
      }
   };
}

// Native entry point (IUIElement::CallFunction("ShowDamage", args, ...) on the
// loaded clip -- same call pattern as FastTravelTweaks' HideFastTravelPath).
this.ShowDamage = function(amount, isHealthDmg, xPx, yPx, sizePx, isFatal)
{
   var isHeal = isHealthDmg && amount < 0;
   var animName = isHealthDmg ? C_ANIM_HEALTHDMG : C_ANIM_STAMINADMG;
   var color;
   if (isHeal)
   {
      color = C_COLOR_HEAL;
   }
   else if (isHealthDmg)
   {
      color = C_COLOR_HEALTH_DMG;
   }
   else
   {
      color = C_COLOR_STAMINA_DMG;
   }
   var shown = isHeal ? ("+" + Math.round(-amount)) : String(Math.round(amount));
   var size = sizePx == undefined ? C_FALLBACK_TEXT_SIZE : sizePx;
   var fatal = isHealthDmg && !isHeal && isFatal == true;
   spawnPopup(animName, shown, color, size, xPx, yPx, fatal);
};
