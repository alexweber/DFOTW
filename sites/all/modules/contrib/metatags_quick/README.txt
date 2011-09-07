***********
* README: *
***********

DESCRIPTION:
------------
Quick and dirty implementation of meta fields, using Drupal 7 Fields API
Module exposes new field type. In most cases, you'll want to create keywords
and description fields. After doing this, you'll be able to add and use meta fields to 
any entity (node, taxonomy term, user etc.)

INSTALLATION:
-------------
1. Place the entire metatags_quick directory into your Drupal sites/all/modules/
   directory.

2. Enable the Meta field module by navigating to:
     administer > modules
     
3. After installation, module suggests to install basic meta tags.
This will create 2 meta tag fields, keywords and description, to
all defined content types. If there are no content types defined
(minimal installation profile), no action is performed.
     
3a. You can skip automatic fields creation and define meta tags
with Field UI module. 
     
4. Add fields of type Meta to any of your content types:
	 administer > structure > Content types > manage fields.
	 
5. (Optional) change meta name (it defaults to the field name)

6. To add fields to taxonomy terms, go to administer > structure
 > taxonomy > select vocabulary > manage fields
 
7. To add fields to user, go to administration > configuration
 > people > manage fields
 
RELATED MODULES

Field UI core module is necessary to define new fields, but can be switched off on 
production servers.

You may want to use field_group module (http://drupal.org/project/field_group)
to add nice grouping of meta tag fields

token module (http://drupal.org/project/token) extends built in D7
token functionality.

---------

Notes:
-----
Meta name is field name. For example, if you add field field_description, resulting meta will be <meta name="description" content="<field value>"/>
You can add several meta fields with different names.


Financial support:
-------
I have started this project as my personal workaround of the meta tags 
problem in Drupal 7. Since then, surprisingly lot of people have downloaded
 and use it, so now it takes more of my time than I have planned initially.
If you find this module useful, please consider helping me to add new features and fix old bugs :)<br>
by donating at http://www.valthebald.net/donate_drupal
Your support is much appreciated. Thanks in advance!

Author:
-------
Valery Lourie valerylourie@gmail.com

