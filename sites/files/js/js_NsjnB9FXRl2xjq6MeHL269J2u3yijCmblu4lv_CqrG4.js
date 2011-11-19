
(function($) {

Drupal.behaviors.fieldUIFieldOverview = {
  attach: function (context, settings) {
    $('table#field-overview', context).once('field-overview', function () {
      Drupal.fieldUIFieldOverview.attachUpdateSelects(this, settings);
    });
  }
};

Drupal.fieldUIFieldOverview = {
  /**
   * Implements dependent select dropdowns on the 'Manage fields' screen.
   */
  attachUpdateSelects: function(table, settings) {
    var widgetTypes = settings.fieldWidgetTypes;
    var fields = settings.fields;

    // Store the default text of widget selects.
    $('.widget-type-select', table).each(function () {
      this.initialValue = this.options[0].text;
    });

    // 'Field type' select updates its 'Widget' select.
    $('.field-type-select', table).each(function () {
      this.targetSelect = $('.widget-type-select', $(this).parents('tr').eq(0));

      $(this).bind('change keyup', function () {
        var selectedFieldType = this.options[this.selectedIndex].value;
        var options = (selectedFieldType in widgetTypes ? widgetTypes[selectedFieldType] : []);
        this.targetSelect.fieldUIPopulateOptions(options);
      });

      // Trigger change on initial pageload to get the right widget options
      // when field type comes pre-selected (on failed validation).
      $(this).trigger('change', false);
    });

    // 'Existing field' select updates its 'Widget' select and 'Label' textfield.
    $('.field-select', table).each(function () {
      this.targetSelect = $('.widget-type-select', $(this).parents('tr').eq(0));
      this.targetTextfield = $('.label-textfield', $(this).parents('tr').eq(0));

      $(this).bind('change keyup', function (e, updateText) {
        var updateText = (typeof updateText == 'undefined' ? true : updateText);
        var selectedField = this.options[this.selectedIndex].value;
        var selectedFieldType = (selectedField in fields ? fields[selectedField].type : null);
        var selectedFieldWidget = (selectedField in fields ? fields[selectedField].widget : null);
        var options = (selectedFieldType && (selectedFieldType in widgetTypes) ? widgetTypes[selectedFieldType] : []);
        this.targetSelect.fieldUIPopulateOptions(options, selectedFieldWidget);

        if (updateText) {
          $(this.targetTextfield).attr('value', (selectedField in fields ? fields[selectedField].label : ''));
        }
      });

      // Trigger change on initial pageload to get the right widget options
      // and label when field type comes pre-selected (on failed validation).
      $(this).trigger('change', false);
    });
  }
};

/**
 * Populates options in a select input.
 */
jQuery.fn.fieldUIPopulateOptions = function (options, selected) {
  return this.each(function () {
    var disabled = false;
    if (options.length == 0) {
      options = [this.initialValue];
      disabled = true;
    }

    // If possible, keep the same widget selected when changing field type.
    // This is based on textual value, since the internal value might be
    // different (options_buttons vs. node_reference_buttons).
    var previousSelectedText = this.options[this.selectedIndex].text;

    var html = '';
    jQuery.each(options, function (value, text) {
      // Figure out which value should be selected. The 'selected' param
      // takes precedence.
      var is_selected = ((typeof selected != 'undefined' && value == selected) || (typeof selected == 'undefined' && text == previousSelectedText));
      html += '<option value="' + value + '"' + (is_selected ? ' selected="selected"' : '') + '>' + text + '</option>';
    });

    $(this).html(html).attr('disabled', disabled ? 'disabled' : '');
  });
};

Drupal.behaviors.fieldUIDisplayOverview = {
  attach: function (context, settings) {
    $('table#field-display-overview', context).once('field-display-overview', function() {
      Drupal.fieldUIOverview.attach(this, settings.fieldUIRowsData, Drupal.fieldUIDisplayOverview);
    });
  }
};

Drupal.fieldUIOverview = {
  /**
   * Attaches the fieldUIOverview behavior.
   */
  attach: function (table, rowsData, rowHandlers) {
    var tableDrag = Drupal.tableDrag[table.id];

    // Add custom tabledrag callbacks.
    tableDrag.onDrop = this.onDrop;
    tableDrag.row.prototype.onSwap = this.onSwap;

    // Create row handlers.
    $('tr.draggable', table).each(function () {
      // Extract server-side data for the row.
      var row = this;
      if (row.id in rowsData) {
        var data = rowsData[row.id];
        data.tableDrag = tableDrag;

        // Create the row handler, make it accessible from the DOM row element.
        var rowHandler = eval('new rowHandlers.' + data.rowHandler + '(row, data);');
        $(row).data('fieldUIRowHandler', rowHandler);
      }
    });
  },

  /**
   * Event handler to be attached to form inputs triggering a region change.
   */
  onChange: function () {
    var $trigger = $(this);
    var row = $trigger.parents('tr:first').get(0);
    var rowHandler = $(row).data('fieldUIRowHandler');

    var refreshRows = {};
    refreshRows[rowHandler.name] = $trigger.get(0);

    // Handle region change.
    var region = rowHandler.getRegion();
    if (region != rowHandler.region) {
      // Remove parenting.
      $('select.field-parent', row).val('');
      // Let the row handler deal with the region change.
      $.extend(refreshRows, rowHandler.regionChange(region));
      // Update the row region.
      rowHandler.region = region;
    }

    // Ajax-update the rows.
    Drupal.fieldUIOverview.AJAXRefreshRows(refreshRows);
  },

  /**
   * Lets row handlers react when a row is dropped into a new region.
   */
  onDrop: function () {
    var dragObject = this;
    var row = dragObject.rowObject.element;
    var rowHandler = $(row).data('fieldUIRowHandler');
    if (rowHandler !== undefined) {
      var regionRow = $(row).prevAll('tr.region-message').get(0);
      var region = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');

      if (region != rowHandler.region) {
        // Let the row handler deal with the region change.
        refreshRows = rowHandler.regionChange(region);
        // Update the row region.
        rowHandler.region = region;
        // Ajax-update the rows.
        Drupal.fieldUIOverview.AJAXRefreshRows(refreshRows);
      }
    }
  },

  /**
   * Refreshes placeholder rows in empty regions while a row is being dragged.
   *
   * Copied from block.js.
   *
   * @param table
   *   The table DOM element.
   * @param rowObject
   *   The tableDrag rowObject for the row being dragged.
   */
  onSwap: function (draggedRow) {
    var rowObject = this;
    $('tr.region-message', rowObject.table).each(function () {
      // If the dragged row is in this region, but above the message row, swap
      // it down one space.
      if ($(this).prev('tr').get(0) == rowObject.group[rowObject.group.length - 1]) {
        // Prevent a recursion problem when using the keyboard to move rows up.
        if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
          rowObject.swap('after', this);
        }
      }
      // This region has become empty.
      if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').length == 0) {
        $(this).removeClass('region-populated').addClass('region-empty');
      }
      // This region has become populated.
      else if ($(this).is('.region-empty')) {
        $(this).removeClass('region-empty').addClass('region-populated');
      }
    });
  },

  /**
   * Triggers Ajax refresh of selected rows.
   *
   * The 'format type' selects can trigger a series of changes in child rows.
   * The #ajax behavior is therefore not attached directly to the selects, but
   * triggered manually through a hidden #ajax 'Refresh' button.
   *
   * @param rows
   *   A hash object, whose keys are the names of the rows to refresh (they
   *   will receive the 'ajax-new-content' effect on the server side), and
   *   whose values are the DOM element in the row that should get an Ajax
   *   throbber.
   */
  AJAXRefreshRows: function (rows) {
    // Separate keys and values.
    var rowNames = [];
    var ajaxElements = [];
    $.each(rows, function (rowName, ajaxElement) {
      rowNames.push(rowName);
      ajaxElements.push(ajaxElement);
    });

    if (rowNames.length) {
      // Add a throbber next each of the ajaxElements.
      var $throbber = $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');
      $(ajaxElements)
        .addClass('progress-disabled')
        .after($throbber);

      // Fire the Ajax update.
      $('input[name=refresh_rows]').val(rowNames.join(' '));
      $('input#edit-refresh').mousedown();

      // Disabled elements do not appear in POST ajax data, so we mark the
      // elements disabled only after firing the request.
      $(ajaxElements).attr('disabled', true);
    }
  }
};


/**
 * Row handlers for the 'Manage display' screen.
 */
Drupal.fieldUIDisplayOverview = {};

/**
 * Constructor for a 'field' row handler.
 *
 * This handler is used for both fields and 'extra fields' rows.
 *
 * @param row
 *   The row DOM element.
 * @param data
 *   Additional data to be populated in the constructed object.
 */
Drupal.fieldUIDisplayOverview.field = function (row, data) {
  this.row = row;
  this.name = data.name;
  this.region = data.region;
  this.tableDrag = data.tableDrag;

  // Attach change listener to the 'formatter type' select.
  this.$formatSelect = $('select.field-formatter-type', row);
  this.$formatSelect.change(Drupal.fieldUIOverview.onChange);

  return this;
};

Drupal.fieldUIDisplayOverview.field.prototype = {
  /**
   * Returns the region corresponding to the current form values of the row.
   */
  getRegion: function () {
    return (this.$formatSelect.val() == 'hidden') ? 'hidden' : 'visible';
  },

  /**
   * Reacts to a row being changed regions.
   *
   * This function is called when the row is moved to a different region, as a
   * result of either :
   * - a drag-and-drop action (the row's form elements then probably need to be
   *   updated accordingly)
   * - user input in one of the form elements watched by the
   *   Drupal.fieldUIOverview.onChange change listener.
   *
   * @param region
   *   The name of the new region for the row.
   * @return
   *   A hash object indicating which rows should be Ajax-updated as a result
   *   of the change, in the format expected by
   *   Drupal.displayOverview.AJAXRefreshRows().
   */
  regionChange: function (region) {

    // When triggered by a row drag, the 'format' select needs to be adjusted
    // to the new region.
    var currentValue = this.$formatSelect.val();
    switch (region) {
      case 'visible':
        if (currentValue == 'hidden') {
          // Restore the formatter back to the default formatter. Pseudo-fields do
          // not have default formatters, we just return to 'visible' for those.
          var value = (this.defaultFormatter != undefined) ? this.defaultFormatter : 'visible';
        }
        break;

      default:
        var value = 'hidden';
        break;
    }
    if (value != undefined) {
      this.$formatSelect.val(value);
    }

    var refreshRows = {};
    refreshRows[this.name] = this.$formatSelect.get(0);

    return refreshRows;
  }
};

})(jQuery);
;

(function($) {

Drupal.behaviors.fieldUIFieldsOverview = {
  attach: function (context, settings) {
    $('table#field-overview', context).once('field-field-overview', function() {
      Drupal.fieldUIOverview.attach(this, settings.fieldUIRowsData, Drupal.fieldUIFieldOverview);
    });
  }
};
  
/**
 * Row handlers for the 'Manage fields' screen.
 */
Drupal.fieldUIFieldOverview = Drupal.fieldUIFieldOverview || {};

Drupal.fieldUIFieldOverview.group = function(row, data) {
  this.row = row;
  this.name = data.name;
  this.region = data.region;
  this.tableDrag = data.tableDrag;

  // Attach change listener to the 'group format' select.
  this.$formatSelect = $('select.field-group-type', row);
  this.$formatSelect.change(Drupal.fieldUIOverview.onChange);

  return this;
};

Drupal.fieldUIFieldOverview.group.prototype = {
  getRegion: function () {
    return 'main';
  },
  
  regionChange: function (region, recurse) {
    return {};
  },

  regionChangeFields: function (region, element, refreshRows) {

    // Create a new tabledrag rowObject, that will compute the group's child
    // rows for us.
    var tableDrag = element.tableDrag;
    rowObject = new tableDrag.row(element.row, 'mouse', true);
    // Skip the main row, we handled it above.
    rowObject.group.shift();

    // Let child rows handlers deal with the region change - without recursing
    // on nested group rows, we are handling them all here.
    $.each(rowObject.group, function() {
      var childRow = this;
      var childRowHandler = $(childRow).data('fieldUIRowHandler');
      $.extend(refreshRows, childRowHandler.regionChange(region, false));
    });
  }  
};
  
  
/**
 * Row handlers for the 'Manage display' screen.
 */
Drupal.fieldUIDisplayOverview = Drupal.fieldUIDisplayOverview || {};

Drupal.fieldUIDisplayOverview.group = function(row, data) {
  this.row = row;
  this.name = data.name;
  this.region = data.region;
  this.tableDrag = data.tableDrag;

  // Attach change listener to the 'group format' select.
  this.$formatSelect = $('select.field-group-type', row);
  this.$formatSelect.change(Drupal.fieldUIOverview.onChange);

  return this;
};

Drupal.fieldUIDisplayOverview.group.prototype = {
  getRegion: function () {
    return (this.$formatSelect.val() == 'hidden') ? 'hidden' : 'visible';
  },

  regionChange: function (region, recurse) {

    // Default recurse to true.
    recurse = (recurse == undefined) || recurse;

    // When triggered by a row drag, the 'format' select needs to be adjusted to
    // the new region.
    var currentValue = this.$formatSelect.val();
    switch (region) {
      case 'visible':
        if (currentValue == 'hidden') {
          // Restore the group format back to 'fieldset'.
          var value = 'fieldset';
        }
        break;

      default:
        var value = 'hidden';
        break;
    }
    if (value != undefined) {
      this.$formatSelect.val(value);
    }

    var refreshRows = {};
    refreshRows[this.name] = this.$formatSelect.get(0);

    if (recurse) {
      this.regionChangeFields(region, this, refreshRows);
    }

    return refreshRows;
  },

  regionChangeFields: function (region, element, refreshRows) {

    // Create a new tabledrag rowObject, that will compute the group's child
    // rows for us.
    var tableDrag = element.tableDrag;
    rowObject = new tableDrag.row(element.row, 'mouse', true);
    // Skip the main row, we handled it above.
    rowObject.group.shift();

    // Let child rows handlers deal with the region change - without recursing
    // on nested group rows, we are handling them all here.
    $.each(rowObject.group, function() {
      var childRow = this;
      var childRowHandler = $(childRow).data('fieldUIRowHandler');
      $.extend(refreshRows, childRowHandler.regionChange(region, false));
    });
    
  }
  
};

})(jQuery);;
(function ($) {

/**
 * Retrieves the summary for the first element.
 */
$.fn.drupalGetSummary = function () {
  var callback = this.data('summaryCallback');
  return (this[0] && callback) ? $.trim(callback(this[0])) : '';
};

/**
 * Sets the summary for all matched elements.
 *
 * @param callback
 *   Either a function that will be called each time the summary is
 *   retrieved or a string (which is returned each time).
 */
$.fn.drupalSetSummary = function (callback) {
  var self = this;

  // To facilitate things, the callback should always be a function. If it's
  // not, we wrap it into an anonymous function which just returns the value.
  if (typeof callback != 'function') {
    var val = callback;
    callback = function () { return val; };
  }

  return this
    .data('summaryCallback', callback)
    // To prevent duplicate events, the handlers are first removed and then
    // (re-)added.
    .unbind('formUpdated.summary')
    .bind('formUpdated.summary', function () {
      self.trigger('summaryUpdated');
    })
    // The actual summaryUpdated handler doesn't fire when the callback is
    // changed, so we have to do this manually.
    .trigger('summaryUpdated');
};

/**
 * Sends a 'formUpdated' event each time a form element is modified.
 */
Drupal.behaviors.formUpdated = {
  attach: function (context) {
    // These events are namespaced so that we can remove them later.
    var events = 'change.formUpdated click.formUpdated blur.formUpdated keyup.formUpdated';
    $(context)
      // Since context could be an input element itself, it's added back to
      // the jQuery object and filtered again.
      .find(':input').andSelf().filter(':input')
      // To prevent duplicate events, the handlers are first removed and then
      // (re-)added.
      .unbind(events).bind(events, function () {
        $(this).trigger('formUpdated');
      });
  }
};

/**
 * Prepopulate form fields with information from the visitor cookie.
 */
Drupal.behaviors.fillUserInfoFromCookie = {
  attach: function (context, settings) {
    $('form.user-info-from-cookie').once('user-info-from-cookie', function () {
      var formContext = this;
      $.each(['name', 'mail', 'homepage'], function () {
        var $element = $('[name=' + this + ']', formContext);
        var cookie = $.cookie('Drupal.visitor.' + this);
        if ($element.length && cookie) {
          $element.val(cookie);
        }
      });
    });
  }
};

})(jQuery);
;
(function ($) {

/**
 * Toggle the visibility of a fieldset using smooth animations.
 */
Drupal.toggleFieldset = function (fieldset) {
  var $fieldset = $(fieldset);
  if ($fieldset.is('.collapsed')) {
    var $content = $('> .fieldset-wrapper', fieldset).hide();
    $fieldset
      .removeClass('collapsed')
      .trigger({ type: 'collapsed', value: false })
      .find('> legend span.fieldset-legend-prefix').html(Drupal.t('Hide'));
    $content.slideDown({
      duration: 'fast',
      easing: 'linear',
      complete: function () {
        Drupal.collapseScrollIntoView(fieldset);
        fieldset.animating = false;
      },
      step: function () {
        // Scroll the fieldset into view.
        Drupal.collapseScrollIntoView(fieldset);
      }
    });
  }
  else {
    $fieldset.trigger({ type: 'collapsed', value: true });
    $('> .fieldset-wrapper', fieldset).slideUp('fast', function () {
      $fieldset
        .addClass('collapsed')
        .find('> legend span.fieldset-legend-prefix').html(Drupal.t('Show'));
      fieldset.animating = false;
    });
  }
};

/**
 * Scroll a given fieldset into view as much as possible.
 */
Drupal.collapseScrollIntoView = function (node) {
  var h = document.documentElement.clientHeight || document.body.clientHeight || 0;
  var offset = document.documentElement.scrollTop || document.body.scrollTop || 0;
  var posY = $(node).offset().top;
  var fudge = 55;
  if (posY + node.offsetHeight + fudge > h + offset) {
    if (node.offsetHeight > h) {
      window.scrollTo(0, posY);
    }
    else {
      window.scrollTo(0, posY + node.offsetHeight - h + fudge);
    }
  }
};

Drupal.behaviors.collapse = {
  attach: function (context, settings) {
    $('fieldset.collapsible', context).once('collapse', function () {
      var $fieldset = $(this);
      // Expand fieldset if there are errors inside, or if it contains an
      // element that is targeted by the uri fragment identifier. 
      var anchor = location.hash && location.hash != '#' ? ', ' + location.hash : '';
      if ($('.error' + anchor, $fieldset).length) {
        $fieldset.removeClass('collapsed');
      }

      var summary = $('<span class="summary"></span>');
      $fieldset.
        bind('summaryUpdated', function () {
          var text = $.trim($fieldset.drupalGetSummary());
          summary.html(text ? ' (' + text + ')' : '');
        })
        .trigger('summaryUpdated');

      // Turn the legend into a clickable link, but retain span.fieldset-legend
      // for CSS positioning.
      var $legend = $('> legend .fieldset-legend', this);

      $('<span class="fieldset-legend-prefix element-invisible"></span>')
        .append($fieldset.hasClass('collapsed') ? Drupal.t('Show') : Drupal.t('Hide'))
        .prependTo($legend)
        .after(' ');

      // .wrapInner() does not retain bound events.
      var $link = $('<a class="fieldset-title" href="#"></a>')
        .prepend($legend.contents())
        .appendTo($legend)
        .click(function () {
          var fieldset = $fieldset.get(0);
          // Don't animate multiple times.
          if (!fieldset.animating) {
            fieldset.animating = true;
            Drupal.toggleFieldset(fieldset);
          }
          return false;
        });

      $legend.append(summary);
    });
  }
};

})(jQuery);
;
