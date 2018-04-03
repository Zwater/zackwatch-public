module.exports = [
    {
        "type": "heading",
        "defaultValue": "ZackWatch Configuration"
    },
    {
        "type": "section",
        "items": [
        {
            "type": "heading",
            "defaultValue": "Watch Settings"
        },
        {
            "type": "toggle",
            "messageKey": "metric",
            "label": "Celcius"
        },
        {
            "type": "input",
            "messageKey": "apikey",
            "label": "Forcast.IO API Key"
        },
        {
            "type": "toggle",
            "messageKey": "defaultLocOnly",
            "label":"Use Provided Lat & Long"
        },
        {
            "type": "input",
            "messageKey": "latitude",
            "label": "Latitude"
        },
        {
            "type": "input",
            "messageKey": "longitude",
            "label": "Longitude"
        },
        {
            "type": "slider",
            "messageKey": "refresh",
            "label": "Refresh Interval",
            "defaultValue": "10"
        }
        ]
    },
    {
        "type": "section",
        "items": [
         {
             "type": "heading",
             "defaultValue": "Weasley Settings"
         },
        {
            "type": "toggle",
            "messageKey": "ppldisable",
            "label": "Hide Weasley Clock",
            "defaultValue": true
        },
        {
            "type": "input",
            "messageKey": "ppl",
            "label": "JSON URL"
        },
        {
            "type": "toggle",
            "messageKey": "vibrate",
            "label": "Vibration",
            "defaultValue": true
        },
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save Settings"
    }
];
