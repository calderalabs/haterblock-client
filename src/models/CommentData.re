module Comment = {
  type t = {
    id: int,
    body: string,
    score: int,
    videoId: int
  };
};

module Sentiment = {
  type t =
  | Hateful
  | Negative
  | Neutral
  | Positive;

  exception InvalidSentiment;

  let sentimentMap = score =>
    switch score {
    | 10
    | 9
    | 8
    | 7
    | 6
    | 5 => Positive
    | 4
    | 3
    | 2
    | 1
    | 0
    | (-1)
    | (-2)
    | (-3) => Neutral
    | (-4)
    | (-5)
    | (-6) => Negative
    | (-7)
    | (-8)
    | (-9)
    | (-10) => Hateful
    | _ => raise(InvalidSentiment)
    };

  let sentiment = (comment: Comment.t) => sentimentMap(comment.score);
};

module CommentDecoder =
  JsonApi.MakeDecoder(
    {
      type model = Comment.t;
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

let fetchAll = (callback: array(Comment.t) => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~callback=json => callback(json |> CommentDecoder.decodeMany),
    ()
  );
