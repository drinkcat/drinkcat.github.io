---
layout: post
title: "Nikon Charger vs \"Nikon\" Charger"
date: 2013-01-18 20:34:00+08:00
categories: others blogspot
---

<!-- TODO: update dell-charger link once 2012-12-dell-charger-vs-dell-charger is migrated -->
On the topic of [charger vs charger](http://drinkcat.blogspot.com/2012/12/dell-charger-vs-dell-charger.html), and how they are not all born equal, here is another one.

This one is a battery charger, for a Nikon Coolpix camera. The original one refused to charge, while in India. The Nikon centre over there opened it, and apparently fixed it. However, it still refused to charge the battery, so we bought another one (on [eBay](http://www.ebay.com/itm/150853166862), again...).

Let's compare the 2 chargers. From the outside, they look pretty similar:

{% include img.html src="/images/nikon-charger-vs-nikon-charger/nikon12-exterior.JPG" alt="Left: eBay charger. Right: original charger." %}

Same shape, same markings on the front. Different labels on the back, but both of them look serious. Now the differences: yellow LED on the counterfeit charger, versus orange LED on the real one. The font of the logo is wrong, but it is not obvious to see without a magnifying lens (not as obvious as the Dell one):

{% include img.html src="/images/nikon-charger-vs-nikon-charger/nikon-logo.JPG" alt="Top: eBay. Bottom: Original." %}

Also, the text in French has a few spelling mistakes, and words are not hyphenated properly (but this could happen even on an original product...).

[This page](http://support.nikonusa.com/app/answers/detail/a_id/14297/%7E/counterfeit-nikon-accessories) on Nikon's website gives a few indications of things to look at (LED color, font of the Nikon logo...): it seems I got a "standard" counterfeit charger.

But the real difference is inside... The original charger says "Do not disassemble", and is glued together, but since the service in India opened it, I didn't feel too guilty... (careful of residual charges in the high voltage capacitors!) The eBay charger provides with a screw: easy to open.

The difference is very obvious:

{% include img.html src="/images/nikon-charger-vs-nikon-charger/nikon12-interior.JPG" alt="Back and front of the PCB. Left: eBay. Right: Original." %}

The fake charger uses a small PCB, with a limited number of components. The battery contacts are badly glued, and broke after a few uses: you can see my attempt at glueing it back in place.

The real one is much more complicated, with a proper separation between high and low voltage parts. It even features a Cypress [microcontroller](http://www.cypress.com/?mpn=CY8C24223A-12PVXE), that, alone, is almost as expensive as the eBay charger (3.20$ for the Cypress chip, 4.74$ for the charger).

Both chargers seem to be able to do their job properly, but I'm pretty sure that the eBay charger has little protection against overcharging or other mishaps. So, in the worse case, keep an eye on it while it's charging (not too close, just in case...), in the best case, don't use it!

<!-- TODO: update dell-charger link once 2012-12-dell-charger-vs-dell-charger is migrated -->
Well, [same morale of the story](http://drinkcat.blogspot.com/2012/12/dell-charger-vs-dell-charger.html): you get what you pay for. Don't be fooled by the fact the charger you're buying says "Nikon", "Dell", or any other brand.
