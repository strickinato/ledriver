### webriver ###
Browser thingy to control LEDRIVER


#### Interface Maker ####
Parse json to make interfaces

```
{
    address: "/foo/",
    args: [
        {
            type: "i",
            input: "range",
            min: 0,
            max: 127,
            name: "speed",
            description: "adjust playback speed"
        }

    ]
}
```

```
{
    "div" : {
        "float" : "left",
        "otherStyle" : "otherStyleThing",
        "widgets" : [
            {
                address: "/foo/",
                args: [
                    {
                        type: "i",
                        input: "range",
                        min: 0,
                        max: 127,
                        name: "speed",
                        description: "adjust playback speed"
                    }

                ]
            },
            {
                address: "/haha/",
                args: [
                    {
                        type: "i",
                        input: "range",
                        min: 0,
                        max: 127,
                        name: "speed",
                        description: "adjust playback speed"
                    }

                ]
            }
        ]
    }
}
```
