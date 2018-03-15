type t = {
  id: int,
  body: string,
  score: int,
  videoId: int
};

type sentiment =
  | Hateful
  | Negative
  | Neutral
  | Positive;

module CommentDecoder =
  JsonApi.MakeDecoder(
    {
      type model = t;
      type attributes = {body: string};
      let attributesDecoder = (json: Js.Json.t) : attributes =>
        Json.Decode.{body: json |> field("body", string)};
      let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : model =>
        switch resource.attributes {
        | None => {id: resource.id, body: "", score: 0, videoId: 0}
        | Some(attributes) => {
            id: resource.id,
            body: attributes.body,
            score: 0,
            videoId: 0
          }
        };
    }
  );

let fetchAll = (callback: array(t) => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~callback=json => callback(json |> CommentDecoder.decodeMany),
    ()
  );
