CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Known Issues
 * Compatibility
 * Authors
 * Sponsors

INTRODUCTION
------------

The HeadJS module uses the head.js (http://headjs.com/) javascript library to dramatically improve javascript loading times by adding only one script to the document's head and then downloading individual script files in parallel.

Since downloading many small files at the same time is faster than downloading one big file, head.js makes pages load blazingly fast and is especially beneficial for certain mobile browsers who limit the size of individually cached javascript files.

For more information see: http://headjs.com/#theory


REQUIREMENTS
------------

* head.js library (0.96+)


INSTALLATION
------------

1. Copy the headjs directory to your sites/all/modules or sites/all/modules/contrib directory.

2. * Download the head.js library from:

     http://headjs.com/#download

   * Extract the files to sites/all/libraries/headjs

   * The actual head.js files should be located in:

     /sites/all/libraries/headjs/head.min.js

     and

     /sites/all/libraries/headjs/head.load.min.js

     OR

     optionally, they could be placed in the "dist" subdirectory of headjs (for backwards compatibility & drush make friendlines)

     /sites/all/libraries/headjs/dist/head.min.js
     /sites/all/libraries/headjs/dist/head.load.min.js

3. Enable the module at Administer >> Site building >> Modules.

4. Visit Administer >> Site Configuration >> Performance >> HeadJS to configure the module (optional)

5. That's it!


USAGE
------

HeadJS is a plug-and-play module. Install it, adjust the settings if you want and forget about it. Add javascript normally using drupal_add_js(), add it to your theme's .info or even include it inline in your templates and HeadJS will detect it and replace it with the head.js version!


SUPPORT
-------

If you find a bug or have a feature request please file an issue: http://drupal.org/node/add/project-issue/headjs

COMPATIBILITY
-------------

A lot of work has gone into ensuring maximum compatibility with other contrib modules. If you find a bug please use the issue tracker for support. Thanks!


AUTHORS
--------

* Alex Weber (alexweber) - http://drupal.org/user/850856 & http://www.alexweber.com.br
* Leandro Nunes (lnunesbr) - http://drupal.org/user/324393 & http://www.nunesweb.com


SPONSORS
---------

This project is made possible by Webdrop (http://webdrop.net.br)

Webdrop is an up and coming Drupal shop based in São Paulo, Brazil. Try us for your next project!
